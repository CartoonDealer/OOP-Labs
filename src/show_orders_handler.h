#pragma once
#include "handler.h"

class ShowOrdersHandler : public Handler {
public:
    explicit ShowOrdersHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
