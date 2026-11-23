#include "create_order_command.h"
#include "shop_facade.h"

CreateOrderCommand::CreateOrderCommand(ShopFacade& facade, const std::string& customer, const std::vector<OrderItemRequest>& items, DeliveryType delivery_type)
    : Command(facade), customer_(customer), items_(items), delivery_type_(delivery_type) {
}

OperationResult CreateOrderCommand::do_execute() {
    return facade_.create_order(customer_, items_, delivery_type_);
}
