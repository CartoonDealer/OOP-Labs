#pragma once
#include <string>
#include <utility>
#include "operation_status.h"

// Результат операции: статус, текст для пользователя и id созданной/изменённой сущности
struct OperationResult {
    OperationStatus status;
    std::string message;
    int entity_id;

    static OperationResult success(std::string msg = "", int id = -1) {
        return { OperationStatus::SUCCESS, std::move(msg), id };
    }

    static OperationResult failure(OperationStatus st, std::string msg) {
        return { st, std::move(msg), -1 };
    }

    bool is_success() const { return status == OperationStatus::SUCCESS; }
};
