#pragma once

struct OrderStatistics {
    int total_orders = 0;
    int pending_orders = 0;
    int paid_orders = 0;
    int shipped_orders = 0;
    int delivered_orders = 0;
    int cancelled_orders = 0;
    int reserved_units = 0; // штук товара, которые сейчас зарезервированы активными заказами
    long long revenue = 0;  // сумма оплаченных (не отменённых) заказов
};
