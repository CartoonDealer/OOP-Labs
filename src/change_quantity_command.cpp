#include "change_quantity_command.h"
#include "shop_facade.h"

ChangeQuantityCommand::ChangeQuantityCommand(ShopFacade& facade, int product_id, int delta)
    : Command(facade), product_id_(product_id), delta_(delta) {
}

OperationResult ChangeQuantityCommand::do_execute() {
    return facade_.change_quantity(product_id_, delta_);
}

OperationResult ChangeQuantityCommand::do_undo() {
    return facade_.change_quantity(product_id_, -delta_);
}
