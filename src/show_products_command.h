#pragma once
#include "command.h"

class ShowProductsCommand : public Command {
private:

protected:
    OperationResult do_execute() override;

public:
    explicit ShowProductsCommand(ShopFacade& facade);

    bool should_be_stored_in_history() const override { return false; }
};
