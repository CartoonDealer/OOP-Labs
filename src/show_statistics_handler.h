#pragma once
#include "handler.h"

class ShowStatisticsHandler : public Handler {
public:
    explicit ShowStatisticsHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
