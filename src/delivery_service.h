#pragma once
#include <string>
#include "delivery_type.h"
#include "operation_result.h"

// Служба доставки: ограниченное число курьеров, пунктов выдачи и почтовых отделений
class DeliveryService {
private:
    int couriers_;
    int pickup_points_;
    int post_offices_;

    int* slots_for(DeliveryType type);
    const int* slots_for(DeliveryType type) const;

public:
    DeliveryService(int couriers = 3, int pickup_points = 5, int post_offices = 10);

    bool is_available(DeliveryType type) const;
    int calculate_cost(DeliveryType type, int items_sum) const;
    OperationResult schedule_delivery(int order_id, DeliveryType type);
    void release_delivery(DeliveryType type);
};
