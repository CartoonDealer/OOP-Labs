#include "shop_mediator.h"
#include <map>
#include <sstream>

ShopMediator::ShopMediator(Warehouse& warehouse, OrderManager& order_manager,
    PaymentSystem& payment_system, DeliveryService& delivery_service)
    : warehouse_(warehouse), order_manager_(order_manager), payment_system_(payment_system),
    delivery_service_(delivery_service), products_added_(0), products_changed_(0), products_removed_(0) {
}

void ShopMediator::release_stock(const std::vector<OrderItem>& items, size_t count) {
    for (size_t i = 0; i < count && i < items.size(); ++i) {
        warehouse_.change_quantity(items[i].get_product_id(), items[i].get_quantity());
    }
}

OperationResult ShopMediator::create_order(const std::string& customer,
    const std::vector<OrderItemRequest>& requests, DeliveryType delivery_type) {
    if (customer.empty()) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, "Не указан клиент");
    }
    if (requests.empty()) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, "Заказ не содержит товаров");
    }

    // Одинаковые товары в запросе складываем
    std::map<int, int> wanted;
    for (const auto& request : requests) {
        if (request.quantity <= 0) {
            return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, "Количество товара в заказе должно быть больше 0");
        }
        wanted[request.product_id] += request.quantity;
    }

    if (!delivery_service_.is_available(delivery_type)) {
        return OperationResult::failure(OperationStatus::FAILED_UNAVAILABLE,
            "Доставка '" + delivery_type_to_string(delivery_type) + "' сейчас недоступна");
    }

    // 1. Проверяем склад и готовим строки заказа
    std::vector<OrderItem> items;
    int items_sum = 0;
    for (const auto& pair : wanted) {
        const Product* product = warehouse_.find_by_id(pair.first);
        if (!product) {
            return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Товар с id " + std::to_string(pair.first) + " не найден");
        }
        if (product->get_quantity() < pair.second) {
            return OperationResult::failure(OperationStatus::FAILED_INSUFFICIENT_STOCK,
                "Недостаточно товара '" + product->get_name() + "': нужно " + std::to_string(pair.second) +
                ", на складе " + std::to_string(product->get_quantity()));
        }
        items.emplace_back(pair.first, product->get_name(), product->get_price(), pair.second);
        items_sum += product->get_price() * pair.second;
    }

    // 2. Резервируем товар на складе (с откатом при ошибке)
    std::ostringstream trace;
    for (size_t i = 0; i < items.size(); ++i) {
        OperationResult reserved = warehouse_.change_quantity(items[i].get_product_id(), -items[i].get_quantity());
        if (!reserved.is_success()) {
            release_stock(items, i);
            return reserved;
        }
        trace << "\n  [Посредник -> Склад] " << reserved.message;
    }

    // 3. Регистрируем заказ
    int total = items_sum + delivery_service_.calculate_cost(delivery_type, items_sum);
    OperationResult registered = order_manager_.create_order(customer, items, delivery_type, total);
    if (!registered.is_success()) {
        release_stock(items, items.size());
        return registered;
    }
    int order_id = registered.entity_id;
    trace << "\n  [Посредник -> Заказы] " << registered.message;

    // 4. Планируем доставку
    OperationResult scheduled = delivery_service_.schedule_delivery(order_id, delivery_type);
    if (!scheduled.is_success()) {
        order_manager_.cancel_order(order_id);
        release_stock(items, items.size());
        return scheduled;
    }
    trace << "\n  [Посредник -> Доставка] " << scheduled.message;

    // 5. Запрашиваем оплату: проверяем, хватит ли денег; списание — по команде confirm payment
    int balance = payment_system_.check_balance(customer);
    trace << "\n  [Посредник -> Оплата] Баланс клиента " << customer << ": " << balance << " руб., к оплате: " << total << " руб.";
    if (balance < total) trace << " (средств пока недостаточно!)";

    return OperationResult::success("Заказ #" + std::to_string(order_id) + " создан, итого " +
        std::to_string(total) + " руб. Ожидает оплаты." + trace.str(), order_id);
}

OperationResult ShopMediator::confirm_payment(int order_id) {
    const Order* order = order_manager_.find_order(order_id);
    if (!order) {
        return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Заказ #" + std::to_string(order_id) + " не найден");
    }
    if (order->get_status() != OrderStatus::PENDING) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_STATE,
            "Оплатить можно только заказ в статусе PENDING, сейчас: " + order_status_to_string(order->get_status()));
    }

    OperationResult payment = payment_system_.process_payment(order->get_customer(), order->get_total_price());
    if (!payment.is_success()) return payment;

    OperationResult confirmed = order_manager_.confirm_payment(order_id);
    if (!confirmed.is_success()) {
        payment_system_.refund(order->get_customer(), order->get_total_price());
        return confirmed;
    }
    return OperationResult::success(confirmed.message + "\n  [Посредник -> Оплата] " + payment.message, order_id);
}

OperationResult ShopMediator::ship_order(int order_id) {
    const Order* order = order_manager_.find_order(order_id);
    if (!order) {
        return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Заказ #" + std::to_string(order_id) + " не найден");
    }
    if (order->get_status() == OrderStatus::PENDING) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_STATE,
            "Заказ #" + std::to_string(order_id) + " не оплачен, отправка невозможна");
    }
    OperationResult shipped = order_manager_.ship_order(order_id);
    if (!shipped.is_success()) return shipped;

    delivery_service_.release_delivery(order->get_delivery_type());
    return OperationResult::success(shipped.message + "\n  [Посредник -> Доставка] заказ передан в доставку ("
        + delivery_type_to_string(order->get_delivery_type()) + ")", order_id);
}

OperationResult ShopMediator::cancel_order(int order_id) {
    const Order* order = order_manager_.find_order(order_id);
    if (!order) {
        return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Заказ #" + std::to_string(order_id) + " не найден");
    }

    // Запоминаем данные заказа до смены статуса
    bool was_paid = order->get_status() == OrderStatus::PAID;
    std::string customer = order->get_customer();
    int total = order->get_total_price();
    DeliveryType delivery_type = order->get_delivery_type();
    std::vector<OrderItem> items = order->get_items();

    OperationResult cancelled = order_manager_.cancel_order(order_id);
    if (!cancelled.is_success()) return cancelled;

    std::ostringstream trace;
    trace << cancelled.message;
    release_stock(items, items.size());
    trace << "\n  [Посредник -> Склад] резерв снят, товары возвращены на склад";
    delivery_service_.release_delivery(delivery_type);
    trace << "\n  [Посредник -> Доставка] доставка отменена";
    if (was_paid) {
        OperationResult refund = payment_system_.refund(customer, total);
        trace << "\n  [Посредник -> Оплата] " << refund.message;
    }
    return OperationResult::success(trace.str(), order_id);
}

OperationResult ShopMediator::get_statistics() {
    OrderStatistics stats = order_manager_.get_statistics();
    std::ostringstream out;
    out << "=== СТАТИСТИКА МАГАЗИНА ===\n"
        << "Заказов всего: " << stats.total_orders << "\n"
        << "  ожидают оплаты: " << stats.pending_orders << "\n"
        << "  оплачено: " << stats.paid_orders << "\n"
        << "  отправлено: " << stats.shipped_orders << "\n"
        << "  доставлено: " << stats.delivered_orders << "\n"
        << "  отменено: " << stats.cancelled_orders << "\n"
        << "Зарезервировано штук товара: " << stats.reserved_units << "\n"
        << "Выручка (оплаченные заказы): " << stats.revenue << " руб.\n"
        << "Товаров в каталоге: " << known_products_.size() << " (событий склада: добавлено "
        << products_added_ << ", изменено " << products_changed_ << ", удалено " << products_removed_ << ")";
    return OperationResult::success(out.str());
}

void ShopMediator::on_product_added(int product_id) {
    known_products_.insert(product_id);
    ++products_added_;
}

void ShopMediator::on_product_changed(int product_id) {
    known_products_.insert(product_id);
    ++products_changed_;
}

void ShopMediator::on_product_removed(int product_id) {
    known_products_.erase(product_id);
    ++products_removed_;
}

OperationResult ShopMediator::can_remove_product(int product_id) const {
    if (order_manager_.has_active_order_with_product(product_id)) {
        return OperationResult::failure(OperationStatus::FAILED_RESERVED,
            "Товар " + std::to_string(product_id) + " зарезервирован под активный заказ, удалить нельзя");
    }
    return OperationResult::success();
}

OperationResult ShopMediator::list_orders() const {
    return order_manager_.list_orders();
}
