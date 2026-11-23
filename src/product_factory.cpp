#include "product_factory.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <vector>

namespace {
const std::vector<std::string>& known_types() {
    static const std::vector<std::string> types = {
        "Smartphone", "Laptop", "Tablet", "Headphones", "Food", "Chemical", "Other"
    };
    return types;
}

std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}
}

std::string ProductFactory::normalize_type(const std::string& type) {
    std::string lowered = to_lower(type);
    for (const auto& known : known_types()) {
        if (to_lower(known) == lowered) return known;
    }
    return "";
}

std::string ProductFactory::get_known_types() {
    std::string result;
    for (const auto& known : known_types()) {
        if (!result.empty()) result += ", ";
        result += known;
    }
    return result;
}

std::unique_ptr<Product> ProductFactory::create_product(const std::string& type, const std::string& name,
    int price, int quantity) const {
    std::string category = normalize_type(type);
    if (category.empty()) {
        throw std::invalid_argument("Неизвестный тип товара '" + type + "'. Доступные: " + get_known_types());
    }
    if (name.empty()) {
        throw std::invalid_argument("Название товара не может быть пустым");
    }
    return std::make_unique<Product>(name, category, Price(price), Quantity(quantity));
}
