#include "command_history.h"

void CommandHistory::push(std::unique_ptr<Command> command) {
    if (command) history_.push_back(std::move(command));
}

OperationResult CommandHistory::undo() {
    if (history_.empty()) {
        return OperationResult::failure(OperationStatus::FAILED_EMPTY_HISTORY, "История команд пуста, отменять нечего");
    }
    Command* last = history_.back().get();
    if (!last->is_undoable()) {
        return OperationResult::failure(OperationStatus::FAILED_BLOCKED, "Откат невозможен: последнее действие необратимо");
    }
    OperationResult result = last->undo();
    if (result.is_success()) history_.pop_back();
    return result;
}

Command* CommandHistory::get_last_command() const {
    return history_.empty() ? nullptr : history_.back().get();
}

size_t CommandHistory::size() const { return history_.size(); }

void CommandHistory::clear() { history_.clear(); }
