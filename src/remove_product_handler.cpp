#include "remove_product_handler.h"
#include "remove_product_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> RemoveProductHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "remove", "product")) return pass_to_next(tokens);
    if (tokens.size() != 3) return usage_error("remove product <id>");
    auto product_id = parse_int(tokens[2]);
    if (!product_id.has_value()) return argument_error("ID товара должен быть целым числом");
    return facade_.execute_command(std::make_unique<RemoveProductCommand>(facade_, *product_id));
}
