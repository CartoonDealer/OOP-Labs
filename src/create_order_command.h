#pragma once
#include "command.h"
#include "order_item_request.h"
#include "delivery_type.h"
#include <vector>
#include <string>

class CreateOrderCommand : public Command {
private:
    std::string customer_;
    std::vector<OrderItemRequest> items_;
    DeliveryType delivery_type_;

protected:
    OperationResult do_execute() override;

public:
    explicit CreateOrderCommand(ShopFacade& facade, const std::string& customer, const std::vector<OrderItemRequest>& items, DeliveryType delivery_type);
};
