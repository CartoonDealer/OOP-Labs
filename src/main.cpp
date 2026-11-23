#include <iostream>
#include <string>
#include "delivery_service.h"
#include "logging_proxy.h"
#include "order_manager.h"
#include "payment_system.h"
#include "scenario_runner.h"
#include "shop_facade.h"
#include "shop_mediator.h"
#include "warehouse.h"

#ifdef _WIN32
#include <windows.h>
#endif

static void print_result(const OperationResult& result) {
    if (result.is_success()) {
        if (!result.message.empty()) std::cout << result.message << "\n";
    }
    else {
        std::cout << "Ошибка: " << result.message << "\n";
    }
}

// demo - все сценарии подряд, demo 1..5 - один сценарий
static void run_demo(InputProcessor& shop, const std::string& line) {
    ScenarioRunner runner(shop);
    if (line == "demo") { runner.run_all(); return; }
    std::string arg = line.substr(5);
    if (arg == "1") runner.run_scenario_1();
    else if (arg == "2") runner.run_scenario_2();
    else if (arg == "3") runner.run_scenario_3();
    else if (arg == "4") runner.run_scenario_4();
    else if (arg == "5") runner.run_scenario_5();
    else std::cout << "Формат: demo или demo <1-5>\n";
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    // Подсистемы (друг о друге не знают)
    Warehouse warehouse(3);
    OrderManager order_manager;
    PaymentSystem payment_system;
    DeliveryService delivery_service;

    // Посредник знает подсистемы, фасад знает склад и посредника
    ShopMediator mediator(warehouse, order_manager, payment_system, delivery_service);
    ShopFacade facade(warehouse, mediator);

    // Все действия пользователя идут через логирующий заместитель
    LoggingProxy shop(facade, "shop.log");

    if (argc > 1 && std::string(argv[1]) == "--demo") {
        ScenarioRunner(shop).run_all();
        return 0;
    }

    std::cout << "=== Интернет-магазин ===\nВведите help для списка команд, demo для демонстрации, exit для выхода.\n";
    std::string line;
    while (true) {
        std::cout << "\n> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit" || line == "quit") break;
        if (line == "demo" || line.rfind("demo ", 0) == 0) {
            run_demo(shop, line);
            continue;
        }
        print_result(shop.process_input(line));
    }
    return 0;
}
