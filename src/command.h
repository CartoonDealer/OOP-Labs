#pragma once
#include "operation_result.h"

// Предварительное объявление: команда знает только фасад (но не посредника)
class ShopFacade;

class Command {
protected:
    ShopFacade& facade_;
    bool executed_;

    virtual OperationResult do_execute() = 0;
    virtual OperationResult do_undo();

public:
    explicit Command(ShopFacade& facade);
    virtual ~Command() = default;

    // Шаблонные методы: общая обработка ошибок и флага executed_
    OperationResult execute();
    OperationResult undo();

    virtual bool is_undoable() const { return false; }
    virtual bool should_be_stored_in_history() const { return true; }
};
