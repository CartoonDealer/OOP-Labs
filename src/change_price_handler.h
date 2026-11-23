#pragma once
#include "handler.h"

class ChangePriceHandler : public Handler {
public:
    explicit ChangePriceHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
