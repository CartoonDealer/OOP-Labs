#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "command.h"
#include "command_history.h"
#include "delivery_type.h"
#include "handler.h"
#include "input_processor.h"
#include "operation_result.h"
#include "order_item_request.h"
#include "product.h"
#include "shop_mediator.h"
#include "warehouse.h"

// Фасад: единая точка входа. Бизнес-логики здесь нет — только делегирование:
// товары -> Warehouse (+ события в ShopMediator), заказы -> ShopMediator.
class ShopFacade : public InputProcessor {
private:
    Warehouse& warehouse_;
    ShopMediator& mediator_;
    CommandHistory history_;
    std::vector<std::unique_ptr<Handler>> handlers_;
    Handler* first_handler_;

    void build_chain();

public:
    ShopFacade(Warehouse& warehouse, ShopMediator& mediator);
    ~ShopFacade() override;

    // Ввод пользователя -> цепочка обязанностей -> команда -> execute_command
    OperationResult process_input(const std::string& input) override;

    // Паттерн "Команда": выполнить и положить в историю
    OperationResult execute_command(std::unique_ptr<Command> command);
    OperationResult undo_last();
    size_t get_history_size() const;

    // Товары
    OperationResult add_product(const std::string& type, const std::string& name, int price, int quantity);
    OperationResult change_price(int id, int new_price);
    OperationResult change_quantity(int id, int delta);
    OperationResult move_product(int id, int new_warehouse_id);
    OperationResult remove_product(int id);
    OperationResult list_products();
    std::optional<Product> get_product(int id) const;
    OperationResult restore_product(const Product& snapshot);

    // Заказы
    OperationResult create_order(const std::string& customer, const std::vector<OrderItemRequest>& items, DeliveryType delivery_type);
    OperationResult cancel_order(int order_id);
    OperationResult confirm_payment(int order_id);
    OperationResult ship_order(int order_id);
    OperationResult list_orders();
    OperationResult get_statistics();
};
