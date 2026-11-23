#pragma once
#include <string>

// Строка заказа: снимок названия и цены на момент оформления
class OrderItem {
private:
    int product_id_;
    std::string product_name_;
    int unit_price_;
    int quantity_;

public:
    OrderItem(int product_id, const std::string& product_name, int unit_price, int quantity);

    int get_product_id() const;
    const std::string& get_product_name() const;
    int get_unit_price() const;
    int get_quantity() const;
    int get_subtotal() const;
};
