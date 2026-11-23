#pragma once
#include "handler.h"

class ChangeQuantityHandler : public Handler {
public:
    explicit ChangeQuantityHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
