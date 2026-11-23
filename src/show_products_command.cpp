#include "show_products_command.h"
#include "shop_facade.h"

ShowProductsCommand::ShowProductsCommand(ShopFacade& facade)
    : Command(facade) {
}

OperationResult ShowProductsCommand::do_execute() {
    return facade_.list_products();
}
