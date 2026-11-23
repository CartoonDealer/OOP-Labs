#pragma once
#include <map>
#include <memory>
#include <string>
#include "operation_result.h"
#include "product.h"
#include "product_factory.h"

// Склад: хранит товары по id. Ничего не знает ни о заказах, ни о посреднике
class Warehouse {
private:
    std::map<int, std::unique_ptr<Product>> products_;
    ProductFactory factory_;
    int next_id_;
    int location_count_; // сколько складов (точек хранения) доступно для move_product

public:
    explicit Warehouse(int location_count = 3);

    OperationResult add_product(const std::string& type, const std::string& name, int price, int quantity);
    OperationResult restore_product(const Product& snapshot); // возвращает товар с прежним id (для undo)
    OperationResult remove_product(int id);

    const Product* find_by_id(int id) const;

    OperationResult change_price(int id, int new_price);
    OperationResult change_quantity(int id, int delta);
    OperationResult move_product(int id, int new_warehouse_id);

    OperationResult list_products() const;
    size_t size() const;
    int get_location_count() const;
};
