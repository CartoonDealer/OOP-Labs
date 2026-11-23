// Простые тесты без внешних библиотек: парсинг, история команд, посредник, фасад
#include <iostream>
#include <string>
#include "delivery_service.h"
#include "order_manager.h"
#include "payment_system.h"
#include "shop_facade.h"
#include "shop_mediator.h"
#include "tokenizer.h"
#include "warehouse.h"

static int failed = 0;
static int passed = 0;

static void check(bool condition, const std::string& name) {
    if (condition) { ++passed; std::cout << "[ OK ] " << name << "\n"; }
    else { ++failed; std::cout << "[FAIL] " << name << "\n"; }
}

#define SETUP_SHOP \
    Warehouse warehouse(3); OrderManager order_manager; PaymentSystem payment_system; DeliveryService delivery_service; \
    ShopMediator mediator(warehouse, order_manager, payment_system, delivery_service); \
    ShopFacade facade(warehouse, mediator);

static void test_tokenizer() {
    auto tokens = Tokenizer::tokenize("create order \"Иван Петров\" 1,2 courier");
    check(tokens.has_value() && tokens->size() == 5 && (*tokens)[2] == "Иван Петров", "парсинг: кавычки объединяют слова");
    check(!Tokenizer::tokenize("add product \"oops").has_value(), "парсинг: незакрытая кавычка -> ошибка");
}

static void test_parsing_via_chain() {
    SETUP_SHOP
    check(facade.process_input("blah blah").status == OperationStatus::FAILED_UNKNOWN_COMMAND, "цепочка: неизвестная команда");
    check(facade.process_input("add product Laptop X abc 5").status == OperationStatus::FAILED_INVALID_ARGUMENTS, "цепочка: нечисловая цена");
    check(facade.process_input("add product Toaster X 5 5").status == OperationStatus::FAILED_INVALID_ARGUMENTS, "цепочка: неизвестный тип товара");
    check(facade.process_input("help").is_success(), "цепочка: help");
    check(facade.process_input("add product Smartphone \"iPhone 15\" 100000 5").is_success(), "цепочка: add product");
    check(facade.process_input("change quantity 1 -2").is_success(), "цепочка: отрицательная дельта");
    check(facade.process_input("change quantity 1 -100").status == OperationStatus::FAILED_INVALID_ARGUMENTS, "склад: остаток не может уйти в минус");
    check(facade.process_input("show products").is_success(), "цепочка: show products");
}

static void test_history() {
    SETUP_SHOP
    facade.process_input("add product Laptop \"MacBook Pro\" 200000 3");
    facade.process_input("change price 1 150000");
    check(facade.get_history_size() == 2, "история: две команды сохранены");
    check(facade.undo_last().is_success() && warehouse.find_by_id(1)->get_price() == 200000, "история: undo возвращает цену");
    check(facade.undo_last().is_success() && warehouse.size() == 0, "история: undo добавления удаляет товар");
    check(facade.undo_last().status == OperationStatus::FAILED_EMPTY_HISTORY, "история: undo при пустой истории");

    facade.process_input("add product Food Milk 90 10");
    facade.process_input("remove product 2"); // id не переиспользуются: после undo добавления следующий товар получил id 2
    check(warehouse.size() == 0, "история: товар удалён");
    check(facade.undo_last().is_success() && warehouse.find_by_id(2) != nullptr, "история: undo удаления восстанавливает товар с тем же id");

    facade.process_input("show products");
    check(facade.get_history_size() == 1, "история: show-команды не сохраняются");
}

static void test_mediator() {
    SETUP_SHOP
    facade.process_input("add product Smartphone \"iPhone 15\" 100000 5");
    facade.process_input("add product Laptop \"MacBook Pro\" 200000 3");

    OperationResult created = facade.process_input("create order \"Иван\" 1,2 courier");
    check(created.is_success(), "посредник: заказ создан");
    check(warehouse.find_by_id(1)->get_quantity() == 4 && warehouse.find_by_id(2)->get_quantity() == 2, "посредник: товар зарезервирован на складе");
    check(order_manager.find_order(1)->get_total_price() == 300500, "посредник: итог = товары + доставка");
    check(facade.process_input("remove product 1").status == OperationStatus::FAILED_RESERVED, "посредник: нельзя удалить зарезервированный товар");

    check(facade.process_input("ship order 1").status == OperationStatus::FAILED_INVALID_STATE, "посредник: нельзя отправить неоплаченный");
    check(facade.process_input("confirm payment 1").is_success(), "посредник: оплата");
    check(payment_system.check_balance("Иван") == 1000000 - 300500, "оплата: списание с баланса");
    check(facade.process_input("confirm payment 1").status == OperationStatus::FAILED_INVALID_STATE, "посредник: повторная оплата запрещена");

    check(facade.process_input("cancel order 1").is_success(), "посредник: отмена оплаченного заказа");
    check(warehouse.find_by_id(1)->get_quantity() == 5, "посредник: после отмены товар вернулся на склад");
    check(payment_system.check_balance("Иван") == 1000000, "посредник: после отмены деньги возвращены");
    check(facade.process_input("remove product 1").is_success(), "посредник: после отмены удаление разрешено");

    check(facade.process_input("create order \"Ян\" 99 courier").status == OperationStatus::FAILED_NOT_FOUND, "посредник: несуществующий товар");
    check(facade.process_input("create order \"Ян\" 2:50 post").status == OperationStatus::FAILED_INSUFFICIENT_STOCK, "посредник: не хватает на складе");
    check(warehouse.find_by_id(2)->get_quantity() == 3, "посредник: неудачный заказ не оставляет резерва");
}

static void test_facade_delegation() {
    SETUP_SHOP
    check(facade.add_product("Food", "Хлеб", 45, 15).is_success(), "фасад: add_product делегирует складу");
    check(warehouse.size() == 1, "фасад: товар реально появился на складе");
    check(facade.move_product(1, 2).is_success() && warehouse.find_by_id(1)->get_location() == 2, "фасад: move_product");
    check(facade.move_product(1, 9).status == OperationStatus::FAILED_INVALID_ARGUMENTS, "фасад: нет такого склада");
    check(facade.get_statistics().is_success(), "фасад: статистика через посредника");
}

int main() {
    test_tokenizer();
    test_parsing_via_chain();
    test_history();
    test_mediator();
    test_facade_delegation();
    std::cout << "\nПройдено: " << passed << ", провалено: " << failed << "\n";
    return failed == 0 ? 0 : 1;
}
