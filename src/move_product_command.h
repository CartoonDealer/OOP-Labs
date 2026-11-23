#pragma once
#include "command.h"
#include <optional>

class MoveProductCommand : public Command {
private:
    int product_id_;
    int new_warehouse_id_;
    std::optional<int> old_warehouse_id_;

protected:
    OperationResult do_execute() override;
    OperationResult do_undo() override;

public:
    explicit MoveProductCommand(ShopFacade& facade, int product_id, int new_warehouse_id);

    bool is_undoable() const override { return true; }
};
