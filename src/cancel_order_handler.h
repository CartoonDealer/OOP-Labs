#pragma once
#include "handler.h"

class CancelOrderHandler : public Handler {
public:
    explicit CancelOrderHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
