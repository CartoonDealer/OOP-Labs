#include "order.h"

Order::Order(int id, const std::string& customer, const std::vector<OrderItem>& items,
    DeliveryType delivery_type, int total_price)
    : id_(id), customer_(customer), items_(items), status_(OrderStatus::PENDING),
    delivery_type_(delivery_type), total_price_(total_price) {
}

int Order::get_id() const { return id_; }
const std::string& Order::get_customer() const { return customer_; }
const std::vector<OrderItem>& Order::get_items() const { return items_; }
OrderStatus Order::get_status() const { return status_; }
DeliveryType Order::get_delivery_type() const { return delivery_type_; }
int Order::get_total_price() const { return total_price_; }

int Order::get_total_units() const {
    int total = 0;
    for (const auto& item : items_) total += item.get_quantity();
    return total;
}

bool Order::is_active() const {
    return status_ == OrderStatus::PENDING || status_ == OrderStatus::PAID;
}

bool Order::contains_product(int product_id) const {
    for (const auto& item : items_) {
        if (item.get_product_id() == product_id) return true;
    }
    return false;
}

void Order::set_status(OrderStatus new_status) { status_ = new_status; }

std::string Order::to_string() const {
    std::string text = "Заказ #" + std::to_string(id_) + " | " + customer_ + " | " +
        order_status_to_string(status_) + " | доставка: " + delivery_type_to_string(delivery_type_) +
        " | итого: " + std::to_string(total_price_) + " руб.\n    товары:";
    for (const auto& item : items_) {
        text += " " + item.get_product_name() + " x" + std::to_string(item.get_quantity()) + ";";
    }
    return text;
}
