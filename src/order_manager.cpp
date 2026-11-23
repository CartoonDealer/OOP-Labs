#include "order_manager.h"

OrderManager::OrderManager() : next_id_(1) {}

Order* OrderManager::find_mutable(int id) {
    auto it = orders_.find(id);
    return it == orders_.end() ? nullptr : &it->second;
}

const Order* OrderManager::find_order(int id) const {
    auto it = orders_.find(id);
    return it == orders_.end() ? nullptr : &it->second;
}

OperationResult OrderManager::create_order(const std::string& customer, const std::vector<OrderItem>& items,
    DeliveryType delivery_type, int total_price) {
    if (customer.empty() || items.empty()) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, "Заказ должен содержать клиента и товары");
    }
    int id = next_id_++;
    orders_.emplace(id, Order(id, customer, items, delivery_type, total_price));
    return OperationResult::success("Заказ #" + std::to_string(id) + " зарегистрирован", id);
}

OperationResult OrderManager::cancel_order(int id) {
    Order* order = find_mutable(id);
    if (!order) return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Заказ #" + std::to_string(id) + " не найден");
    if (!order->is_active()) {
        return OperationResult::failure(OperationStatus::FAILED_BLOCKED,
            "Нельзя отменить заказ #" + std::to_string(id) + " в статусе " + order_status_to_string(order->get_status()));
    }
    order->set_status(OrderStatus::CANCELLED);
    return OperationResult::success("Заказ #" + std::to_string(id) + " отменён", id);
}

OperationResult OrderManager::confirm_payment(int id) {
    Order* order = find_mutable(id);
    if (!order) return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Заказ #" + std::to_string(id) + " не найден");
    if (order->get_status() != OrderStatus::PENDING) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_STATE,
            "Оплатить можно только заказ в статусе PENDING, сейчас: " + order_status_to_string(order->get_status()));
    }
    order->set_status(OrderStatus::PAID);
    return OperationResult::success("Заказ #" + std::to_string(id) + " оплачен", id);
}

OperationResult OrderManager::ship_order(int id) {
    Order* order = find_mutable(id);
    if (!order) return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Заказ #" + std::to_string(id) + " не найден");
    if (order->get_status() != OrderStatus::PAID) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_STATE,
            "Отправить можно только оплаченный заказ, сейчас: " + order_status_to_string(order->get_status()));
    }
    order->set_status(OrderStatus::SHIPPED);
    return OperationResult::success("Заказ #" + std::to_string(id) + " отправлен", id);
}

bool OrderManager::has_active_order_with_product(int product_id) const {
    for (const auto& pair : orders_) {
        if (pair.second.is_active() && pair.second.contains_product(product_id)) return true;
    }
    return false;
}

OrderStatistics OrderManager::get_statistics() const {
    OrderStatistics stats;
    for (const auto& pair : orders_) {
        const Order& order = pair.second;
        stats.total_orders++;
        switch (order.get_status()) {
        case OrderStatus::PENDING: stats.pending_orders++; break;
        case OrderStatus::PAID: stats.paid_orders++; break;
        case OrderStatus::SHIPPED: stats.shipped_orders++; break;
        case OrderStatus::DELIVERED: stats.delivered_orders++; break;
        case OrderStatus::CANCELLED: stats.cancelled_orders++; break;
        }
        if (order.is_active()) stats.reserved_units += order.get_total_units();
        if (order.get_status() != OrderStatus::PENDING && order.get_status() != OrderStatus::CANCELLED) {
            stats.revenue += order.get_total_price();
        }
    }
    return stats;
}

OperationResult OrderManager::list_orders() const {
    if (orders_.empty()) return OperationResult::success("Заказов пока нет.");
    std::string text = "=== ЗАКАЗЫ ===";
    for (const auto& pair : orders_) text += "\n  " + pair.second.to_string();
    return OperationResult::success(text);
}
