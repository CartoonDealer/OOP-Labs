#pragma once
#include "handler.h"

class HelpHandler : public Handler {
public:
    explicit HelpHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
