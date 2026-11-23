#pragma once
#include <fstream>
#include <string>
#include "input_processor.h"

// Заместитель (Proxy): оборачивает любой InputProcessor и пишет в лог
// каждое действие пользователя и результат его выполнения.
class LoggingProxy : public InputProcessor {
private:
    InputProcessor& target_;
    std::ofstream log_file_;

    static std::string current_time();
    static std::string single_line(const std::string& text);

public:
    LoggingProxy(InputProcessor& target, const std::string& log_path);

    OperationResult process_input(const std::string& input) override;
};
