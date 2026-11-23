#pragma once
#include "command.h"

class UndoCommand : public Command {
private:

protected:
    OperationResult do_execute() override;

public:
    explicit UndoCommand(ShopFacade& facade);

    bool should_be_stored_in_history() const override { return false; }
};
