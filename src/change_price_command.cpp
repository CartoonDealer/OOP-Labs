#include "change_price_command.h"
#include "shop_facade.h"

ChangePriceCommand::ChangePriceCommand(ShopFacade& facade, int product_id, int new_price)
    : Command(facade), product_id_(product_id), new_price_(new_price) {
}

OperationResult ChangePriceCommand::do_execute() {
    std::optional<Product> product = facade_.get_product(product_id_);
    if (!product.has_value()) {
        return OperationResult::failure(OperationStatus::FAILED_NOT_FOUND, "Товар с id " + std::to_string(product_id_) + " не найден");
    }
    old_price_ = product->get_price();
    return facade_.change_price(product_id_, new_price_);
}

OperationResult ChangePriceCommand::do_undo() {
    if (!old_price_.has_value()) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_STATE, "Невозможно отменить: старая цена неизвестна");
    }
    return facade_.change_price(product_id_, *old_price_);
}
