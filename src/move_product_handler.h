#pragma once
#include "handler.h"

class MoveProductHandler : public Handler {
public:
    explicit MoveProductHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
