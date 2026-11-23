#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include "command.h"
#include "operation_result.h"

// История выполненных команд (стек, LIFO). Владеет командами
class CommandHistory {
private:
    std::vector<std::unique_ptr<Command>> history_;

public:
    void push(std::unique_ptr<Command> command);
    OperationResult undo();
    Command* get_last_command() const;
    size_t size() const;
    void clear();
};
