#include "create_order_handler.h"
#include "create_order_command.h"
#include "delivery_type.h"
#include "shop_facade.h"
#include <memory>

bool CreateOrderHandler::parse_items(const std::string& text, std::vector<OrderItemRequest>& out) {
    size_t start = 0;
    while (start <= text.size()) {
        size_t end = text.find(',', start);
        if (end == std::string::npos) end = text.size();
        std::string part = text.substr(start, end - start);

        std::string id_text = part;
        std::string quantity_text = "1";
        size_t colon = part.find(':');
        if (colon != std::string::npos) {
            id_text = part.substr(0, colon);
            quantity_text = part.substr(colon + 1);
        }
        auto product_id = parse_int(id_text);
        auto quantity = parse_int(quantity_text);
        if (!product_id.has_value() || !quantity.has_value() || *quantity <= 0) return false;
        out.push_back({ *product_id, *quantity });

        start = end + 1;
    }
    return !out.empty();
}

std::optional<OperationResult> CreateOrderHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "create", "order")) return pass_to_next(tokens);
    if (tokens.size() != 5) return usage_error("create order <customer> <id[:qty],id[:qty],...> <courier|pickup|post>");
    if (tokens[2].empty()) return argument_error("Имя клиента не может быть пустым");

    std::vector<OrderItemRequest> items;
    if (!parse_items(tokens[3], items)) {
        return argument_error("Товары задаются списком id через запятую (например 1,2 или 1:3,2), количество > 0");
    }
    DeliveryType delivery_type;
    if (!delivery_type_from_string(tokens[4], delivery_type)) {
        return argument_error("Неизвестный тип доставки '" + tokens[4] + "'. Доступны: courier, pickup, post");
    }
    return facade_.execute_command(std::make_unique<CreateOrderCommand>(facade_, tokens[2], items, delivery_type));
}
