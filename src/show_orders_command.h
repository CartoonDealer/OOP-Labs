#pragma once
#include "command.h"

class ShowOrdersCommand : public Command {
private:

protected:
    OperationResult do_execute() override;

public:
    explicit ShowOrdersCommand(ShopFacade& facade);

    bool should_be_stored_in_history() const override { return false; }
};
