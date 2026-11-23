#include "price.h"
#include <stdexcept>

Price::Price() : value_(0) {}

Price::Price(int value) : value_(value) {
    if (value < 0) throw std::invalid_argument("Цена не может быть отрицательной");
}

int Price::get_value() const { return value_; }

void Price::set_value(int new_value) {
    if (new_value < 0) throw std::invalid_argument("Цена не может быть отрицательной");
    value_ = new_value;
}
