#pragma once
#include "handler.h"

class ShipOrderHandler : public Handler {
public:
    explicit ShipOrderHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
