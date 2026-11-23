#pragma once
#include "command.h"

class ShowStatisticsCommand : public Command {
private:

protected:
    OperationResult do_execute() override;

public:
    explicit ShowStatisticsCommand(ShopFacade& facade);

    bool should_be_stored_in_history() const override { return false; }
};
