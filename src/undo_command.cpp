#include "undo_command.h"
#include "shop_facade.h"

UndoCommand::UndoCommand(ShopFacade& facade)
    : Command(facade) {
}

OperationResult UndoCommand::do_execute() {
    return facade_.undo_last();
}
