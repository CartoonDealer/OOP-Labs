#pragma once
#include "command.h"

class ConfirmPaymentCommand : public Command {
private:
    int order_id_;

protected:
    OperationResult do_execute() override;

public:
    explicit ConfirmPaymentCommand(ShopFacade& facade, int order_id);
};
