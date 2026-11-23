#include "logging_proxy.h"
#include <ctime>

LoggingProxy::LoggingProxy(InputProcessor& target, const std::string& log_path)
    : target_(target), log_file_(log_path, std::ios::app) {
}

std::string LoggingProxy::current_time() {
    std::time_t now = std::time(nullptr);
    std::tm local_tm;
#ifdef _WIN32
    localtime_s(&local_tm, &now);
#else
    localtime_r(&now, &local_tm);
#endif
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_tm);
    return buffer;
}

std::string LoggingProxy::single_line(const std::string& text) {
    std::string result = text;
    for (char& c : result) {
        if (c == '\n' || c == '\r') c = ' ';
    }
    return result;
}

OperationResult LoggingProxy::process_input(const std::string& input) {
    if (log_file_.is_open()) {
        log_file_ << "[" << current_time() << "] INPUT:  " << input << std::endl;
    }

    OperationResult result = target_.process_input(input);

    if (log_file_.is_open()) {
        log_file_ << "[" << current_time() << "] RESULT: " << operation_status_to_string(result.status)
            << " | " << single_line(result.message) << std::endl;
    }
    return result;
}
