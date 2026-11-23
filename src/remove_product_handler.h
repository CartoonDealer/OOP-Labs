#pragma once
#include "handler.h"

class RemoveProductHandler : public Handler {
public:
    explicit RemoveProductHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
