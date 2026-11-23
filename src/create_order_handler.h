#pragma once
#include "handler.h"
#include "order_item_request.h"

class CreateOrderHandler : public Handler {
private:
    // "1,2" или "1:3,2:1" (id[:количество]) -> список запросов; false при ошибке формата
    static bool parse_items(const std::string& text, std::vector<OrderItemRequest>& out);

public:
    explicit CreateOrderHandler(ShopFacade& facade) : Handler(facade) {}
    std::optional<OperationResult> handle(const std::vector<std::string>& tokens) override;
};
