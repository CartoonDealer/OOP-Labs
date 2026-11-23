#pragma once
#include <string>
#include "input_processor.h"

// Демонстрационные сценарии 1-5 из задания: команды подаются так же, как вводил бы пользователь
class ScenarioRunner {
private:
    InputProcessor& processor_;

    void run_line(const std::string& line);
    void print_title(const std::string& title);

public:
    explicit ScenarioRunner(InputProcessor& processor);

    void run_scenario_1(); // добавление товаров и просмотр склада
    void run_scenario_2(); // заказ с доставкой и оплатой
    void run_scenario_3(); // изменение цены и остатка (+ undo)
    void run_scenario_4(); // отмена и отправка заказа, статистика
    void run_scenario_5(); // координация через посредника
    void run_all();
};
