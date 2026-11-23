#pragma once
#include "handler.h"

class ShowProductsHandler : public Handler {
public:
    explicit ShowProductsHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
