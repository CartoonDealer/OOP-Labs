#include "ship_order_handler.h"
#include "ship_order_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> ShipOrderHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "ship", "order")) return pass_to_next(tokens);
    if (tokens.size() != 3) return usage_error("ship order <id>");
    auto order_id = parse_int(tokens[2]);
    if (!order_id.has_value()) return argument_error("ID заказа должен быть целым числом");
    return facade_.execute_command(std::make_unique<ShipOrderCommand>(facade_, *order_id));
}
