#include "ship_order_command.h"
#include "shop_facade.h"

ShipOrderCommand::ShipOrderCommand(ShopFacade& facade, int order_id)
    : Command(facade), order_id_(order_id) {
}

OperationResult ShipOrderCommand::do_execute() {
    return facade_.ship_order(order_id_);
}
