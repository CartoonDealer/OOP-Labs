#include "remove_product_command.h"
#include "shop_facade.h"

RemoveProductCommand::RemoveProductCommand(ShopFacade& facade, int product_id)
    : Command(facade), product_id_(product_id) {
}

OperationResult RemoveProductCommand::do_execute() {
    snapshot_ = facade_.get_product(product_id_);
    if (!snapshot_.has_value()) {
        return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Товар с id " + std::to_string(product_id_) + " не найден");
    }
    return facade_.remove_product(product_id_);
}

OperationResult RemoveProductCommand::do_undo() {
    if (!snapshot_.has_value()) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_STATE, "Невозможно отменить: снимок товара отсутствует");
    }
    return facade_.restore_product(*snapshot_);
}
