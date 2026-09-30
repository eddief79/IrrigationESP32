#pragma once
#include "driver/gpio.h"

class Valve
{
    public:
        Valve();
        Valve(gpio_num_t control_pin_1, gpio_num_t control_pin_2, gpio_num_t driver_enable_pin);
        void Open();
        void Close();
    private:
        gpio_num_t control_pin_1_;
        gpio_num_t control_pin_2_;
        gpio_num_t driver_enable_pin_;
};