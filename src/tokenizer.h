#pragma once
#include <optional>
#include <string>
#include <vector>

// Разбивает строку на токены с учётом кавычек: create order "Иван Петров" 1,2 courier
// Пустые кавычки "" дают пустой токен. Возвращает nullopt, если кавычка не закрыта.
class Tokenizer {
public:
    static std::optional<std::vector<std::string>> tokenize(const std::string& input);
};
