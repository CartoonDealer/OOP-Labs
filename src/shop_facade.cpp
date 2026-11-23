#include "shop_facade.h"
#include "tokenizer.h"
#include "help_handler.h"
#include "add_product_handler.h"
#include "change_price_handler.h"
#include "change_quantity_handler.h"
#include "move_product_handler.h"
#include "remove_product_handler.h"
#include "create_order_handler.h"
#include "cancel_order_handler.h"
#include "confirm_payment_handler.h"
#include "ship_order_handler.h"
#include "show_statistics_handler.h"
#include "show_products_handler.h"
#include "show_orders_handler.h"
#include "undo_handler.h"

ShopFacade::ShopFacade(Warehouse& warehouse, ShopMediator& mediator)
    : warehouse_(warehouse), mediator_(mediator), first_handler_(nullptr) {
    build_chain();
}

ShopFacade::~ShopFacade() = default;

void ShopFacade::build_chain() {
    handlers_.push_back(std::make_unique<HelpHandler>(*this));
    handlers_.push_back(std::make_unique<AddProductHandler>(*this));
    handlers_.push_back(std::make_unique<ChangePriceHandler>(*this));
    handlers_.push_back(std::make_unique<ChangeQuantityHandler>(*this));
    handlers_.push_back(std::make_unique<MoveProductHandler>(*this));
    handlers_.push_back(std::make_unique<RemoveProductHandler>(*this));
    handlers_.push_back(std::make_unique<CreateOrderHandler>(*this));
    handlers_.push_back(std::make_unique<CancelOrderHandler>(*this));
    handlers_.push_back(std::make_unique<ConfirmPaymentHandler>(*this));
    handlers_.push_back(std::make_unique<ShipOrderHandler>(*this));
    handlers_.push_back(std::make_unique<ShowStatisticsHandler>(*this));
    handlers_.push_back(std::make_unique<ShowProductsHandler>(*this));
    handlers_.push_back(std::make_unique<ShowOrdersHandler>(*this));
    handlers_.push_back(std::make_unique<UndoHandler>(*this));

    for (size_t i = 0; i + 1 < handlers_.size(); ++i) {
        handlers_[i]->set_next(handlers_[i + 1].get());
    }
    first_handler_ = handlers_.front().get();
}

OperationResult ShopFacade::process_input(const std::string& input) {
    auto tokens = Tokenizer::tokenize(input);
    if (!tokens.has_value()) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, "Незакрытая кавычка в команде");
    }
    if (tokens->empty()) return OperationResult::success("");

    auto result = first_handler_->handle(*tokens);
    if (!result.has_value()) {
        return OperationResult::failure(OperationStatus::FAILED_UNKNOWN_COMMAND,
            "Неизвестная команда '" + input + "'. Введите help для списка команд");
    }
    return *result;
}

OperationResult ShopFacade::execute_command(std::unique_ptr<Command> command) {
    if (!command) {
        return OperationResult::failure(OperationStatus::FAILED_INVALID_ARGUMENTS, "Передана пустая команда");
    }
    OperationResult result = command->execute();
    if (result.is_success() && command->should_be_stored_in_history()) {
        history_.push(std::move(command));
    }
    return result;
}

OperationResult ShopFacade::undo_last() {
    return history_.undo();
}

size_t ShopFacade::get_history_size() const {
    return history_.size();
}

OperationResult ShopFacade::add_product(const std::string& type, const std::string& name, int price, int quantity) {
    OperationResult result = warehouse_.add_product(type, name, price, quantity);
    if (result.is_success()) mediator_.on_product_added(result.entity_id);
    return result;
}

OperationResult ShopFacade::change_price(int id, int new_price) {
    OperationResult result = warehouse_.change_price(id, new_price);
    if (result.is_success()) mediator_.on_product_changed(id);
    return result;
}

OperationResult ShopFacade::change_quantity(int id, int delta) {
    OperationResult result = warehouse_.change_quantity(id, delta);
    if (result.is_success()) mediator_.on_product_changed(id);
    return result;
}

OperationResult ShopFacade::move_product(int id, int new_warehouse_id) {
    OperationResult result = warehouse_.move_product(id, new_warehouse_id);
    if (result.is_success()) mediator_.on_product_changed(id);
    return result;
}

OperationResult ShopFacade::remove_product(int id) {
    OperationResult allowed = mediator_.can_remove_product(id);
    if (!allowed.is_success()) return allowed;

    OperationResult result = warehouse_.remove_product(id);
    if (result.is_success()) mediator_.on_product_removed(id);
    return result;
}

OperationResult ShopFacade::list_products() {
    return warehouse_.list_products();
}

std::optional<Product> ShopFacade::get_product(int id) const {
    const Product* product = warehouse_.find_by_id(id);
    if (!product) return std::nullopt;
    return *product;
}

OperationResult ShopFacade::restore_product(const Product& snapshot) {
    OperationResult result = warehouse_.restore_product(snapshot);
    if (result.is_success()) mediator_.on_product_added(snapshot.get_id());
    return result;
}

OperationResult ShopFacade::create_order(const std::string& customer, const std::vector<OrderItemRequest>& items, DeliveryType delivery_type) {
    return mediator_.create_order(customer, items, delivery_type);
}

OperationResult ShopFacade::cancel_order(int order_id) { return mediator_.cancel_order(order_id); }
OperationResult ShopFacade::confirm_payment(int order_id) { return mediator_.confirm_payment(order_id); }
OperationResult ShopFacade::ship_order(int order_id) { return mediator_.ship_order(order_id); }
OperationResult ShopFacade::list_orders() { return mediator_.list_orders(); }
OperationResult ShopFacade::get_statistics() { return mediator_.get_statistics(); }
