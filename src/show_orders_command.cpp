#include "show_orders_command.h"
#include "shop_facade.h"

ShowOrdersCommand::ShowOrdersCommand(ShopFacade& facade)
    : Command(facade) {
}

OperationResult ShowOrdersCommand::do_execute() {
    return facade_.list_orders();
}
