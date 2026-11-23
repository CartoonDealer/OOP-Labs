#include "change_price_handler.h"
#include "change_price_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> ChangePriceHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "change", "price")) return pass_to_next(tokens);
    if (tokens.size() != 4) return usage_error("change price <id> <new_price>");
    auto product_id = parse_int(tokens[2]);
    if (!product_id.has_value()) return argument_error("ID товара должен быть целым числом");
    auto new_price = parse_int(tokens[3]);
    if (!new_price.has_value() || *new_price < 0) return argument_error("цена должен быть целым неотрицательным числом");
    return facade_.execute_command(std::make_unique<ChangePriceCommand>(facade_, *product_id, *new_price));
}
