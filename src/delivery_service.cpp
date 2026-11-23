#include "delivery_service.h"

DeliveryService::DeliveryService(int couriers, int pickup_points, int post_offices)
    : couriers_(couriers), pickup_points_(pickup_points), post_offices_(post_offices) {
}

int* DeliveryService::slots_for(DeliveryType type) {
    switch (type) {
    case DeliveryType::COURIER: return &couriers_;
    case DeliveryType::PICKUP: return &pickup_points_;
    case DeliveryType::POST: return &post_offices_;
    }
    return nullptr;
}

const int* DeliveryService::slots_for(DeliveryType type) const {
    return const_cast<DeliveryService*>(this)->slots_for(type);
}

bool DeliveryService::is_available(DeliveryType type) const {
    const int* slots = slots_for(type);
    return slots && *slots > 0;
}

int DeliveryService::calculate_cost(DeliveryType type, int items_sum) const {
    (void)items_sum;
    switch (type) {
    case DeliveryType::COURIER: return 500;
    case DeliveryType::POST: return 200;
    case DeliveryType::PICKUP: return 0;
    }
    return 0;
}

OperationResult DeliveryService::schedule_delivery(int order_id, DeliveryType type) {
    int* slots = slots_for(type);
    if (!slots || *slots <= 0) {
        return OperationResult::failure(OperationStatus::FAILED_UNAVAILABLE,
            "Доставка '" + delivery_type_to_string(type) + "' сейчас недоступна");
    }
    --(*slots);
    std::string eta;
    switch (type) {
    case DeliveryType::COURIER: eta = "в течение дня"; break;
    case DeliveryType::PICKUP: eta = "1-3 дня"; break;
    case DeliveryType::POST: eta = "5-10 дней"; break;
    }
    return OperationResult::success("Доставка заказа #" + std::to_string(order_id) + " запланирована (" +
        delivery_type_to_string(type) + ", срок: " + eta + ")", order_id);
}

void DeliveryService::release_delivery(DeliveryType type) {
    int* slots = slots_for(type);
    if (slots) ++(*slots);
}
