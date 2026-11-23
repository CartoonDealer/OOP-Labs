#include "cancel_order_handler.h"
#include "cancel_order_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> CancelOrderHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "cancel", "order")) return pass_to_next(tokens);
    if (tokens.size() != 3) return usage_error("cancel order <id>");
    auto order_id = parse_int(tokens[2]);
    if (!order_id.has_value()) return argument_error("ID заказа должен быть целым числом");
    return facade_.execute_command(std::make_unique<CancelOrderCommand>(facade_, *order_id));
}
