#include "handler.h"
#include <charconv>

Handler::Handler(ShopFacade& facade) : next_(nullptr), facade_(facade) {}

Handler* Handler::set_next(Handler* next_handler) {
    next_ = next_handler;
    return next_handler;
}

std::optional<int> Handler::parse_int(const std::string& token) {
    int value = 0;
    const char* begin = token.data();
    const char* end = token.data() + token.size();
    if (!token.empty() && token[0] == '+') ++begin; // from_chars не принимает '+'
    auto [ptr, ec] = std::from_chars(begin, end, value);
    if (ec == std::errc() && ptr == end && begin != end) return value;
    return std::nullopt;
}

bool Handler::starts_with(const std::vector<std::string>& tokens, const std::string& first, const std::string& second) {
    if (tokens.empty() || tokens[0] != first) return false;
    if (second.empty()) return true;
    return tokens.size() >= 2 && tokens[1] == second;
}

OperationResult Handler::usage_error(const std::string& usage) {
    return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS,
        "Неверное количество аргументов. Формат: " + usage);
}

OperationResult Handler::argument_error(const std::string& message) {
    return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, message);
}

std::optional<OperationResult> Handler::pass_to_next(const std::vector<std::string>& tokens) {
    if (next_) return next_->handle(tokens);
    return std::nullopt;
}
