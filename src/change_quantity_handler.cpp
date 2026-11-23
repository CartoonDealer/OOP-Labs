#include "change_quantity_handler.h"
#include "change_quantity_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> ChangeQuantityHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "change", "quantity")) return pass_to_next(tokens);
    if (tokens.size() != 4) return usage_error("change quantity <id> <delta>");
    auto product_id = parse_int(tokens[2]);
    if (!product_id.has_value()) return argument_error("ID товара должен быть целым числом");
    auto delta = parse_int(tokens[3]);
    if (!delta.has_value()) return argument_error("изменение остатка должен быть целым числом");
    return facade_.execute_command(std::make_unique<ChangeQuantityCommand>(facade_, *product_id, *delta));
}
