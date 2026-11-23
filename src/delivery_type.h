#pragma once
#include <string>

enum class DeliveryType {
    COURIER,
    PICKUP,
    POST
};

inline std::string delivery_type_to_string(DeliveryType type) {
    switch (type) {
    case DeliveryType::COURIER: return "courier";
    case DeliveryType::PICKUP: return "pickup";
    case DeliveryType::POST: return "post";
    default: return "unknown";
    }
}

// Возвращает true и записывает результат, если строка — известный тип доставки
inline bool delivery_type_from_string(const std::string& text, DeliveryType& out) {
    if (text == "courier") { out = DeliveryType::COURIER; return true; }
    if (text == "pickup") { out = DeliveryType::PICKUP; return true; }
    if (text == "post") { out = DeliveryType::POST; return true; }
    return false;
}
