#include "command.h"
#include <exception>

Command::Command(ShopFacade& facade) : facade_(facade), executed_(false) {}

OperationResult Command::do_undo() {
    return OperationResult::failure(OperationStatus::FAILED_BLOCKED, "Отмена не поддерживается");
}

OperationResult Command::execute() {
    if (executed_) {
        return OperationResult::failure(OperationStatus::FAILED_BLOCKED, "Команда уже была выполнена");
    }
    try {
        OperationResult result = do_execute();
        if (result.is_success()) executed_ = true;
        return result;
    }
    catch (const std::exception& e) {
        return OperationResult::failure(OperationStatus::FAILED_OTHER, std::string("Непредвиденная ошибка: ") + e.what());
    }
    catch (...) {
        return OperationResult::failure(OperationStatus::FAILED_OTHER, "Критическая ошибка");
    }
}

OperationResult Command::undo() {
    if (!executed_) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_STATE, "Нечего отменять");
    }
    try {
        OperationResult result = do_undo();
        if (result.is_success()) executed_ = false;
        return result;
    }
    catch (const std::exception& e) {
        return OperationResult::failure(OperationStatus::FAILED_OTHER, std::string("Ошибка при откате: ") + e.what());
    }
    catch (...) {
        return OperationResult::failure(OperationStatus::FAILED_OTHER, "Критическая ошибка при откате");
    }
}
