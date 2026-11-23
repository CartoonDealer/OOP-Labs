#include "confirm_payment_handler.h"
#include "confirm_payment_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> ConfirmPaymentHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "confirm", "payment")) return pass_to_next(tokens);
    if (tokens.size() != 3) return usage_error("confirm payment <id>");
    auto order_id = parse_int(tokens[2]);
    if (!order_id.has_value()) return argument_error("ID заказа должен быть целым числом");
    return facade_.execute_command(std::make_unique<ConfirmPaymentCommand>(facade_, *order_id));
}
