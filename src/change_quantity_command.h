#pragma once
#include "command.h"

class ChangeQuantityCommand : public Command {
private:
    int product_id_;
    int delta_;

protected:
    OperationResult do_execute() override;
    OperationResult do_undo() override;

public:
    explicit ChangeQuantityCommand(ShopFacade& facade, int product_id, int delta);

    bool is_undoable() const override { return true; }
};
