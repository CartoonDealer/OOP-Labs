#include "product.h"

Product::Product(const std::string& name, const std::string& category, const Price& price, const Quantity& quantity)
    : id_(0), name_(name), price_(price), quantity_(quantity), category_(category), location_(1) {
}

int Product::get_id() const { return id_; }
const std::string& Product::get_name() const { return name_; }
int Product::get_price() const { return price_.get_value(); }
int Product::get_quantity() const { return quantity_.get_value(); }
const std::string& Product::get_category() const { return category_; }
int Product::get_location() const { return location_; }

void Product::set_id(int id) { id_ = id; }
void Product::set_location(int location) { location_ = location; }

void Product::change_price(int new_price) { price_.set_value(new_price); }
void Product::change_quantity(int delta) { quantity_.change(delta); }

std::string Product::to_string() const {
    return "[" + std::to_string(id_) + "] " + category_ + " | " + name_ +
        " | " + std::to_string(price_.get_value()) + " руб. | " +
        std::to_string(quantity_.get_value()) + " шт. | склад " + std::to_string(location_);
}
