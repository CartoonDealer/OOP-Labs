#include "show_statistics_handler.h"
#include "show_statistics_command.h"
#include "shop_facade.h"
#include <memory>

std::optional<OperationResult> ShowStatisticsHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "show", "statistics")) return pass_to_next(tokens);
    if (tokens.size() != 2) return usage_error("show statistics");
    return facade_.execute_command(std::make_unique<ShowStatisticsCommand>(facade_));
}
