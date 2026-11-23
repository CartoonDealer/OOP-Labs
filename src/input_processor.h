#pragma once
#include <string>
#include "operation_result.h"

// Общий интерфейс "обработать строку пользователя": его реализуют и фасад, и LoggingProxy
class InputProcessor {
public:
    virtual ~InputProcessor() = default;
    virtual OperationResult process_input(const std::string& input) = 0;
};
