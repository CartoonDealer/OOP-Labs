#pragma once
#include <memory>
#include <string>
#include "product.h"

class ProductFactory {
public:
    // Создаёт товар нужной категории. Бросает std::invalid_argument при неизвестном типе
    std::unique_ptr<Product> create_product(const std::string& type, const std::string& name,
        int price, int quantity) const;

    // Каноническое имя категории или пустая строка, если тип неизвестен
    static std::string normalize_type(const std::string& type);
    static std::string get_known_types();
};
