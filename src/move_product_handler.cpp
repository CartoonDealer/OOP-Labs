#include "move_product_handler.h"
#include "move_product_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> MoveProductHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "move", "product")) return pass_to_next(tokens);
    if (tokens.size() != 5 || tokens[3] != "to") return usage_error("move product <id> to <warehouse_id>");
    auto product_id = parse_int(tokens[2]);
    if (!product_id.has_value()) return argument_error("ID товара должен быть целым числом");
    auto warehouse_id = parse_int(tokens[4]);
    if (!warehouse_id.has_value()) return argument_error("ID склада должен быть целым числом");
    return facade_.execute_command(std::make_unique<MoveProductCommand>(facade_, *product_id, *warehouse_id));
}
