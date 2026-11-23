#pragma once
#include "handler.h"

class UndoHandler : public Handler {
public:
    explicit UndoHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
