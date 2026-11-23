#pragma once
#include "command.h"
#include <optional>
#include <string>

class AddProductCommand : public Command {
private:
    std::string type_;
    std::string name_;
    int price_;
    int quantity_;
    std::optional<int> added_product_id_;

protected:
    OperationResult do_execute() override;
    OperationResult do_undo() override;

public:
    explicit AddProductCommand(ShopFacade& facade, const std::string& type, const std::string& name, int price, int quantity);

    bool is_undoable() const override { return true; }
};
