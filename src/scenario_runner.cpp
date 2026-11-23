#include "scenario_runner.h"
#include <iostream>

ScenarioRunner::ScenarioRunner(InputProcessor& processor) : processor_(processor) {}

void ScenarioRunner::print_title(const std::string& title) {
    std::cout << "\n==================== " << title << " ====================\n";
}

void ScenarioRunner::run_line(const std::string& line) {
    std::cout << "\n> " << line << "\n";
    OperationResult result = processor_.process_input(line);
    if (result.is_success()) {
        if (!result.message.empty()) std::cout << result.message << "\n";
    }
    else {
        std::cout << "Ошибка: " << result.message << "\n";
    }
}

void ScenarioRunner::run_scenario_1() {
    print_title("Сценарий 1: добавление товаров и просмотр склада");
    run_line("add product Smartphone \"iPhone 15\" 100000 5");
    run_line("add product Laptop \"MacBook Pro\" 200000 3");
    run_line("show products");
}

void ScenarioRunner::run_scenario_2() {
    print_title("Сценарий 2: создание заказа с доставкой и оплатой");
    run_line("create order \"Иван\" 1,2 courier");
    run_line("confirm payment 1");
    run_line("show orders");
}

void ScenarioRunner::run_scenario_3() {
    print_title("Сценарий 3: изменение цены и остатка товара");
    run_line("change price 1 90000");
    run_line("change quantity 1 -2");
    run_line("show products");
    std::cout << "\n(демонстрация истории команд: откатываем изменение остатка)\n";
    run_line("undo");
    run_line("show products");
}

void ScenarioRunner::run_scenario_4() {
    print_title("Сценарий 4: отмена и отправка заказа");
    std::cout << "\n(подготовка: второй заказ, чтобы было что отменять)\n";
    run_line("create order \"Пётр\" 1 pickup");
    run_line("cancel order 2");
    run_line("ship order 1");
    run_line("show statistics");
}

void ScenarioRunner::run_scenario_5() {
    print_title("Сценарий 5: координация через посредника");
    run_line("add product Food \"Молоко\" 90 10");
    run_line("create order \"Анна\" 3:4 post");
    std::cout << "\n(товар зарезервирован под заказ - удалить его нельзя)\n";
    run_line("remove product 3");
    run_line("show products");
    run_line("show statistics");
    std::cout << "\n(после отмены заказа резерв снимается и удаление проходит)\n";
    run_line("cancel order 3");
    run_line("remove product 3");
    run_line("show statistics");
}

void ScenarioRunner::run_all() {
    run_scenario_1();
    run_scenario_2();
    run_scenario_3();
    run_scenario_4();
    run_scenario_5();
}
