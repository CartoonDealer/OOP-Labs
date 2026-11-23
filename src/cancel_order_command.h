#pragma once
#include "command.h"

class CancelOrderCommand : public Command {
private:
    int order_id_;

protected:
    OperationResult do_execute() override;

public:
    explicit CancelOrderCommand(ShopFacade& facade, int order_id);
};
