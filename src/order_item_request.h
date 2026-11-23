#pragma once

// Что пользователь хочет заказать: id товара и количество
struct OrderItemRequest {
    int product_id;
    int quantity;
};
