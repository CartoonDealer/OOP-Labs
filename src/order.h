#pragma once
#include <string>
#include <vector>
#include "delivery_type.h"
#include "order_item.h"
#include "order_status.h"

class Order {
private:
    int id_;
    std::string customer_;
    std::vector<OrderItem> items_;
    OrderStatus status_;
    DeliveryType delivery_type_;
    int total_price_;

public:
    Order(int id, const std::string& customer, const std::vector<OrderItem>& items,
        DeliveryType delivery_type, int total_price);

    int get_id() const;
    const std::string& get_customer() const;
    const std::vector<OrderItem>& get_items() const;
    OrderStatus get_status() const;
    DeliveryType get_delivery_type() const;
    int get_total_price() const;
    int get_total_units() const;

    bool is_active() const; // ещё не отправлен и не отменён (товар под резервом)
    bool contains_product(int product_id) const;

    void set_status(OrderStatus new_status);
    std::string to_string() const;
};
