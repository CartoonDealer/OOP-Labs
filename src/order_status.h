#pragma once
#include <string>

enum class OrderStatus {
    PENDING,
    PAID,
    SHIPPED,
    DELIVERED,
    CANCELLED
};

inline std::string order_status_to_string(OrderStatus status) {
    switch (status) {
    case OrderStatus::PENDING: return "PENDING (ожидает оплаты)";
    case OrderStatus::PAID: return "PAID (оплачен)";
    case OrderStatus::SHIPPED: return "SHIPPED (отправлен)";
    case OrderStatus::DELIVERED: return "DELIVERED (доставлен)";
    case OrderStatus::CANCELLED: return "CANCELLED (отменён)";
    default: return "UNKNOWN";
    }
}
