#pragma once
#include "command.h"

class ShipOrderCommand : public Command {
private:
    int order_id_;

protected:
    OperationResult do_execute() override;

public:
    explicit ShipOrderCommand(ShopFacade& facade, int order_id);
};
