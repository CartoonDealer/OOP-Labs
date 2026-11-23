#pragma once
#include "command.h"
#include <optional>

class ChangePriceCommand : public Command {
private:
    int product_id_;
    int new_price_;
    std::optional<int> old_price_;

protected:
    OperationResult do_execute() override;
    OperationResult do_undo() override;

public:
    explicit ChangePriceCommand(ShopFacade& facade, int product_id, int new_price);

    bool is_undoable() const override { return true; }
};
