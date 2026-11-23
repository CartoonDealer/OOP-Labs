#include "cancel_order_command.h"
#include "shop_facade.h"

CancelOrderCommand::CancelOrderCommand(ShopFacade& facade, int order_id)
    : Command(facade), order_id_(order_id) {
}

OperationResult CancelOrderCommand::do_execute() {
    return facade_.cancel_order(order_id_);
}
