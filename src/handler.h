#pragma once
#include <optional>
#include <string>
#include <vector>
#include "operation_result.h"

class ShopFacade;

// Звено цепочки обязанностей: либо обрабатывает введённую команду, либо передаёт её дальше
class Handler {
protected:
    Handler* next_;
    ShopFacade& facade_;

    static std::optional<int> parse_int(const std::string& token);
    static bool starts_with(const std::vector<std::string>& tokens, const std::string& first, const std::string& second = "");
    static OperationResult usage_error(const std::string& usage);
    static OperationResult argument_error(const std::string& message);

    std::optional<OperationResult> pass_to_next(const std::vector<std::string>& tokens);

public:
    explicit Handler(ShopFacade& facade);
    virtual ~Handler() = default;

    // Возвращает переданный обработчик, чтобы цепочку можно было строить через a->set_next(b)->set_next(c)
    Handler* set_next(Handler* next_handler);

    // std::nullopt означает "эту команду никто в цепочке не распознал"
    virtual std::optional<OperationResult> handle(const std::vector<std::string>& tokens) = 0;
};
