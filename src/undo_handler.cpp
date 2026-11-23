#include "undo_handler.h"
#include "undo_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> UndoHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "undo")) return pass_to_next(tokens);
    if (tokens.size() != 1) return usage_error("undo");
    return facade_.execute_command(std::make_unique<UndoCommand>(facade_));
}
