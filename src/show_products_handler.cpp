#include "show_products_handler.h"
#include "show_products_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> ShowProductsHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "show", "products")) return pass_to_next(tokens);
    if (tokens.size() != 2) return usage_error("show products");
    return facade_.execute_command(std::make_unique<ShowProductsCommand>(facade_));
}
