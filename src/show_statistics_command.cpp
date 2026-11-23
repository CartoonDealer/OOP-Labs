#include "show_statistics_command.h"
#include "shop_facade.h"

ShowStatisticsCommand::ShowStatisticsCommand(ShopFacade& facade)
    : Command(facade) {
}

OperationResult ShowStatisticsCommand::do_execute() {
    return facade_.get_statistics();
}
