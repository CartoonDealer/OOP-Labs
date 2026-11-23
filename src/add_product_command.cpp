#include "add_product_command.h"
#include "shop_facade.h"

AddProductCommand::AddProductCommand(ShopFacade& facade, const std::string& type, const std::string& name, int price, int quantity)
    : Command(facade), type_(type), name_(name), price_(price), quantity_(quantity) {
}

OperationResult AddProductCommand::do_execute() {
    OperationResult result = facade_.add_product(type_, name_, price_, quantity_);
    if (result.is_success()) added_product_id_ = result.entity_id;
    return result;
}

OperationResult AddProductCommand::do_undo() {
    if (!added_product_id_.has_value()) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_STATE, "Невозможно отменить: id добавленного товара неизвестен");
    }
    return facade_.remove_product(*added_product_id_);
}
