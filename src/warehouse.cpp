#include "warehouse.h"
#include <algorithm>
#include <stdexcept>

Warehouse::Warehouse(int location_count)
    : next_id_(1), location_count_(std::max(1, location_count)) {
}

OperationResult Warehouse::add_product(const std::string& type, const std::string& name, int price, int quantity) {
    try {
        auto product = factory_.create_product(type, name, price, quantity);
        int id = next_id_++;
        product->set_id(id);
        std::string description = product->to_string();
        products_[id] = std::move(product);
        return OperationResult::success("Товар добавлен: " + description, id);
    }
    catch (const std::invalid_argument& e) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, e.what());
    }
}

OperationResult Warehouse::restore_product(const Product& snapshot) {
    if (products_.count(snapshot.get_id())) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_STATE,
            "Товар с id " + std::to_string(snapshot.get_id()) + " уже существует");
    }
    products_[snapshot.get_id()] = std::make_unique<Product>(snapshot);
    next_id_ = std::max(next_id_, snapshot.get_id() + 1);
    return OperationResult::success("Товар восстановлен: " + snapshot.to_string(), snapshot.get_id());
}

OperationResult Warehouse::remove_product(int id) {
    auto it = products_.find(id);
    if (it == products_.end()) {
        return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Товар с id " + std::to_string(id) + " не найден");
    }
    std::string name = it->second->get_name();
    products_.erase(it);
    return OperationResult::success("Товар удалён: " + name, id);
}

const Product* Warehouse::find_by_id(int id) const {
    auto it = products_.find(id);
    return it == products_.end() ? nullptr : it->second.get();
}

OperationResult Warehouse::change_price(int id, int new_price) {
    auto it = products_.find(id);
    if (it == products_.end()) {
        return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Товар с id " + std::to_string(id) + " не найден");
    }
    try {
        int old_price = it->second->get_price();
        it->second->change_price(new_price);
        return OperationResult::success("Цена товара '" + it->second->get_name() + "': " +
            std::to_string(old_price) + " -> " + std::to_string(new_price) + " руб.", id);
    }
    catch (const std::invalid_argument& e) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, e.what());
    }
}

OperationResult Warehouse::change_quantity(int id, int delta) {
    auto it = products_.find(id);
    if (it == products_.end()) {
        return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Товар с id " + std::to_string(id) + " не найден");
    }
    try {
        int old_quantity = it->second->get_quantity();
        it->second->change_quantity(delta);
        return OperationResult::success("Остаток товара '" + it->second->get_name() + "': " +
            std::to_string(old_quantity) + " -> " + std::to_string(it->second->get_quantity()) + " шт.", id);
    }
    catch (const std::invalid_argument& e) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, e.what());
    }
}

OperationResult Warehouse::move_product(int id, int new_warehouse_id) {
    auto it = products_.find(id);
    if (it == products_.end()) {
        return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Товар с id " + std::to_string(id) + " не найден");
    }
    if (new_warehouse_id < 1 || new_warehouse_id > location_count_) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS,
            "Склада " + std::to_string(new_warehouse_id) + " не существует (доступны 1.." + std::to_string(location_count_) + ")");
    }
    if (it->second->get_location() == new_warehouse_id) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_STATE, "Товар уже находится на этом складе");
    }
    int old_location = it->second->get_location();
    it->second->set_location(new_warehouse_id);
    return OperationResult::success("Товар '" + it->second->get_name() + "' перемещён: склад " +
        std::to_string(old_location) + " -> склад " + std::to_string(new_warehouse_id), id);
}

OperationResult Warehouse::list_products() const {
    if (products_.empty()) return OperationResult::success("Склад пуст.");
    std::string text = "=== ТОВАРЫ НА СКЛАДЕ ===";
    for (const auto& pair : products_) text += "\n  " + pair.second->to_string();
    return OperationResult::success(text);
}

size_t Warehouse::size() const { return products_.size(); }
int Warehouse::get_location_count() const { return location_count_; }
