#pragma once
#include <map>
#include <string>
#include <vector>
#include "delivery_type.h"
#include "operation_result.h"
#include "order.h"
#include "order_statistics.h"

// Хранит заказы и следит за допустимыми переходами статусов.
// С товарами, оплатой и доставкой не работает — этим занимается посредник
class OrderManager {
private:
    std::map<int, Order> orders_;
    int next_id_;

    Order* find_mutable(int id);

public:
    OrderManager();

    OperationResult create_order(const std::string& customer, const std::vector<OrderItem>& items,
        DeliveryType delivery_type, int total_price);
    OperationResult cancel_order(int id);
    OperationResult confirm_payment(int id);
    OperationResult ship_order(int id);

    const Order* find_order(int id) const;
    bool has_active_order_with_product(int product_id) const;

    OrderStatistics get_statistics() const;
    OperationResult list_orders() const;
};
