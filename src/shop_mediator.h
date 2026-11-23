#pragma once
#include <set>
#include <string>
#include <vector>
#include "delivery_service.h"
#include "delivery_type.h"
#include "operation_result.h"
#include "order_item.h"
#include "order_item_request.h"
#include "order_manager.h"
#include "payment_system.h"
#include "warehouse.h"

// Посредник: координирует жизненный цикл заказа между складом, менеджером заказов,
// оплатой и доставкой. Подсистемы о посреднике и друг о друге ничего не знают.
class ShopMediator {
private:
    Warehouse& warehouse_;
    OrderManager& order_manager_;
    PaymentSystem& payment_system_;
    DeliveryService& delivery_service_;

    std::set<int> known_products_; // каталог, как его видит посредник (по событиям)
    int products_added_;
    int products_changed_;
    int products_removed_;

    void release_stock(const std::vector<OrderItem>& items, size_t count);

public:
    ShopMediator(Warehouse& warehouse, OrderManager& order_manager,
        PaymentSystem& payment_system, DeliveryService& delivery_service);

    // Жизненный цикл заказа
    OperationResult create_order(const std::string& customer,
        const std::vector<OrderItemRequest>& requests, DeliveryType delivery_type);
    OperationResult confirm_payment(int order_id);
    OperationResult ship_order(int order_id);
    OperationResult cancel_order(int order_id);
    OperationResult get_statistics();

    // События склада (приходят через фасад)
    void on_product_added(int product_id);
    void on_product_changed(int product_id);
    void on_product_removed(int product_id);

    // Согласованность данных: нельзя удалить товар, зарезервированный под заказ
    OperationResult can_remove_product(int product_id) const;

    // Просмотр заказов тоже идёт через посредника, чтобы фасад не знал про OrderManager
    OperationResult list_orders() const;
};
