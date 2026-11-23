#include "confirm_payment_command.h"
#include "shop_facade.h"

ConfirmPaymentCommand::ConfirmPaymentCommand(ShopFacade& facade, int order_id)
    : Command(facade), order_id_(order_id) {
}

OperationResult ConfirmPaymentCommand::do_execute() {
    return facade_.confirm_payment(order_id_);
}
