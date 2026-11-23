#pragma once
#include <map>
#include <string>
#include "operation_result.h"

// Платёжная система: учебные счета клиентов (новый клиент получает стартовый баланс)
class PaymentSystem {
private:
    std::map<std::string, int> balances_;
    int initial_balance_;

    int& account(const std::string& customer);

public:
    explicit PaymentSystem(int initial_balance = 1000000);

    OperationResult process_payment(const std::string& customer, int amount);
    OperationResult refund(const std::string& customer, int amount);
    int check_balance(const std::string& customer);
};
