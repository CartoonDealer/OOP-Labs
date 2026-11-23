#include "add_product_handler.h"
#include "add_product_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> AddProductHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "add", "product")) return pass_to_next(tokens);
    if (tokens.size() != 6) return usage_error("add product <type> <name> <price> <quantity>");
    auto price = parse_int(tokens[4]);
    if (!price.has_value() || *price < 0) return argument_error("Цена должна быть целым неотрицательным числом");
    auto quantity = parse_int(tokens[5]);
    if (!quantity.has_value() || *quantity < 0) return argument_error("Количество должно быть целым неотрицательным числом");
    return facade_.execute_command(std::make_unique<AddProductCommand>(facade_, tokens[2], tokens[3], *price, *quantity));
}
