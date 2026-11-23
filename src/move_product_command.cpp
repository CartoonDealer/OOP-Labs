#include "move_product_command.h"
#include "shop_facade.h"

MoveProductCommand::MoveProductCommand(ShopFacade& facade, int product_id, int new_warehouse_id)
    : Command(facade), product_id_(product_id), new_warehouse_id_(new_warehouse_id) {
}

OperationResult MoveProductCommand::do_execute() {
    std::optional<Product> product = facade_.get_product(product_id_);
    if (!product.has_value()) {
        return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Товар с id " + std::to_string(product_id_) + " не найден");
    }
    old_warehouse_id_ = product->get_location();
    return facade_.move_product(product_id_, new_warehouse_id_);
}

OperationResult MoveProductCommand::do_undo() {
    if (!old_warehouse_id_.has_value()) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_STATE, "Невозможно отменить: прежний склад неизвестен");
    }
    return facade_.move_product(product_id_, *old_warehouse_id_);
}
