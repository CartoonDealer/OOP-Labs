#pragma once
#include "command.h"
#include "product.h"
#include <optional>

class RemoveProductCommand : public Command {
private:
    int product_id_;
    std::optional<Product> snapshot_;

protected:
    OperationResult do_execute() override;
    OperationResult do_undo() override;

public:
    explicit RemoveProductCommand(ShopFacade& facade, int product_id);

    bool is_undoable() const override { return true; }
};
