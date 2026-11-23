#pragma once

class Price {
private:
    int value_;

public:
    Price();
    explicit Price(int value);

    int get_value() const;
    void set_value(int new_value);
};
