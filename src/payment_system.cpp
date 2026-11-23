#include "payment_system.h"

PaymentSystem::PaymentSystem(int initial_balance) : initial_balance_(initial_balance) {}

int& PaymentSystem::account(const std::string& customer) {
    auto it = balances_.find(customer);
    if (it == balances_.end()) it = balances_.emplace(customer, initial_balance_).first;
    return it->second;
}

OperationResult PaymentSystem::process_payment(const std::string& customer, int amount) {
    if (amount < 0) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, "Сумма платежа не может быть отрицательной");
    }
    int& balance = account(customer);
    if (balance < amount) {
        return OperationResult::failure(OperationStatus::FAILED_INSUFFICIENT_FUNDS,
            "Недостаточно средств у клиента " + customer + ": нужно " + std::to_string(amount) +
            " руб., на счёте " + std::to_string(balance) + " руб.");
    }
    balance -= amount;
    return OperationResult::success("Списано " + std::to_string(amount) + " руб. (остаток " + std::to_string(balance) + " руб.)");
}

OperationResult PaymentSystem::refund(const std::string& customer, int amount) {
    if (amount < 0) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, "Сумма возврата не может быть отрицательной");
    }
    int& balance = account(customer);
    balance += amount;
    return OperationResult::success("Возвращено " + std::to_string(amount) + " руб. (остаток " + std::to_string(balance) + " руб.)");
}

int PaymentSystem::check_balance(const std::string& customer) {
    return account(customer);
}
