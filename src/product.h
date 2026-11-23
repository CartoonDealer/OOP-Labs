#pragma once
#include <string>
#include "price.h"
#include "quantity.h"

class Product {
private:
    int id_;
    std::string name_;
    Price price_;
    Quantity quantity_;
    std::string category_;
    int location_; // номер склада (точки хранения), на котором лежит товар

public:
    Product(const std::string& name, const std::string& category, const Price& price, const Quantity& quantity);

    int get_id() const;
    const std::string& get_name() const;
    int get_price() const;
    int get_quantity() const;
    const std::string& get_category() const;
    int get_location() const;

    void set_id(int id);
    void set_location(int location);

    void change_price(int new_price);
    void change_quantity(int delta);

    std::string to_string() const;
};
