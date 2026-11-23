#pragma once

class Quantity {
private:
    int value_;

public:
    Quantity();
    explicit Quantity(int value);

    int get_value() const;
    void set_value(int new_value);
    void change(int delta);
};
