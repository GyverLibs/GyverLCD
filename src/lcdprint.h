#pragma once
#include <Print.h>

// Arduino Print адаптер поверх любого LCD-слоя с write(uint8_t)
template <typename LCD>
class GyverLCD_Print : public LCD, public Print {
   public:
    using LCD::LCD;
    using LCD::write;
    using Print::print;
    using Print::println;

    size_t write(uint8_t data) override {
        return LCD::write(data);
    }
};
