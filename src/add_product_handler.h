#pragma once
#include "handler.h"

class AddProductHandler : public Handler {
public:
    explicit AddProductHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
