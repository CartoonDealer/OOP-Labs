#include "help_handler.h"

std::optional<OperationResult> HelpHandler::handle(const std::vector<std::string>& tokens) {
    if (!starts_with(tokens, "help")) return pass_to_next(tokens);

    return OperationResult::success(
        "=== СПИСОК ДОСТУПНЫХ КОМАНД ===\n"
        "  help\n"
        "  add product <type> <name> <price> <quantity>   (type: Smartphone, Laptop, Tablet, Headphones, Food, Chemical, Other)\n"
        "  change price <id> <new_price>\n"
        "  change quantity <id> <delta>                    (delta может быть отрицательной)\n"
        "  move product <id> to <warehouse_id>\n"
        "  remove product <id>\n"
        "  create order <customer> <id[:qty],id[:qty],...> <courier|pickup|post>\n"
        "  cancel order <id>\n"
        "  confirm payment <id>\n"
        "  ship order <id>\n"
        "  show products\n"
        "  show orders\n"
        "  show statistics\n"
        "  undo                                            (отмена последней отменяемой команды)\n"
        "  demo [1-5]                                      (демонстрационные сценарии; без номера - все подряд)\n"
        "  exit                                            (выход)\n"
        "Строки с пробелами берите в кавычки: add product Laptop \"MacBook Pro\" 200000 3");
}
