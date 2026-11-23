#include "tokenizer.h"
#include <cctype>

std::optional<std::vector<std::string>> Tokenizer::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::string current;
    bool in_quotes = false;
    bool has_token = false;

    for (char c : input) {
        if (c == '"') {
            in_quotes = !in_quotes;
            has_token = true;
        }
        else if (std::isspace(static_cast<unsigned char>(c)) && !in_quotes) {
            if (has_token) {
                tokens.push_back(current);
                current.clear();
                has_token = false;
            }
        }
        else {
            current += c;
            has_token = true;
        }
    }

    if (in_quotes) return std::nullopt;
    if (has_token) tokens.push_back(current);
    return tokens;
}
