#include "order_item.h"

OrderItem::OrderItem(int product_id, const std::string& product_name, int unit_price, int quantity)
    : product_id_(product_id), product_name_(product_name), unit_price_(unit_price), quantity_(quantity) {
}

int OrderItem::get_product_id() const { return product_id_; }
const std::string& OrderItem::get_product_name() const { return product_name_; }
int OrderItem::get_unit_price() const { return unit_price_; }
int OrderItem::get_quantity() const { return quantity_; }
int OrderItem::get_subtotal() const { return unit_price_ * quantity_; }
