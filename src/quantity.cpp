#include "quantity.h"
#include <stdexcept>

Quantity::Quantity() : value_(0) {}

Quantity::Quantity(int value) : value_(value) {
    if (value < 0) throw std::invalid_argument("Количество не может быть отрицательным");
}

int Quantity::get_value() const { return value_; }

void Quantity::set_value(int new_value) {
    if (new_value < 0) throw std::invalid_argument("Количество не может быть отрицательным");
    value_ = new_value;
}

void Quantity::change(int delta) {
    long long result = static_cast<long long>(value_) + delta;
    if (result < 0) throw std::invalid_argument("Количество не может стать отрицательным");
    value_ = static_cast<int>(result);
}
