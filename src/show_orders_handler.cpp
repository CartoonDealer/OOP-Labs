#include "show_orders_handler.h"
#include "show_orders_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> ShowOrdersHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "show", "orders")) return pass_to_next(tokens);
    if (tokens.size() != 2) return usage_error("show orders");
    return facade_.execute_command(std::make_unique<ShowOrdersCommand>(facade_));
}
