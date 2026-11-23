#pragma once
#include <string>

// Коды результата любой операции магазина
enum class OperationStatus {
    SUCCESS,
    FAILED_INVALID_ARGUMENTS,
    FAILED_UNKNOWN_COMMAND,
    FAILED_INVALID_STATE,
    FAILED_NOT_FOUND,
    FAILED_RESERVED,
    FAILED_UNAVAILABLE,
    FAILED_BLOCKED,
    FAILED_INSUFFICIENT_STOCK,
    FAILED_INSUFFICIENT_FUNDS,
    FAILED_EMPTY_HISTORY,
    FAILED_OTHER
};

inline std::string operation_status_to_string(OperationStatus status) {
    switch (status) {
    case OperationStatus::SUCCESS: return "SUCCESS";
    case OperationStatus::FAILED_INVALID_ARGUMENTS: return "FAILED_INVALID_ARGUMENTS";
    case OperationStatus::FAILED_UNKNOWN_COMMAND: return "FAILED_UNKNOWN_COMMAND";
    case OperationStatus::FAILED_INVALID_STATE: return "FAILED_INVALID_STATE";
    case OperationStatus::FAILED_NOT_FOUND: return "FAILED_NOT_FOUND";
    case OperationStatus::FAILED_RESERVED: return "FAILED_RESERVED";
    case OperationStatus::FAILED_UNAVAILABLE: return "FAILED_UNAVAILABLE";
    case OperationStatus::FAILED_BLOCKED: return "FAILED_BLOCKED";
    case OperationStatus::FAILED_INSUFFICIENT_STOCK: return "FAILED_INSUFFICIENT_STOCK";
    case OperationStatus::FAILED_INSUFFICIENT_FUNDS: return "FAILED_INSUFFICIENT_FUNDS";
    case OperationStatus::FAILED_EMPTY_HISTORY: return "FAILED_EMPTY_HISTORY";
    default: return "FAILED_OTHER";
    }
}
