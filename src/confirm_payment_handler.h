#pragma once
#include "handler.h"

class ConfirmPaymentHandler : public Handler {
public:
    explicit ConfirmPaymentHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
