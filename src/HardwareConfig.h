#pragma once

#include "driver/gpio.h"

namespace Hardware
{
    constexpr gpio_num_t FLOW_METER_PIN = GPIO_NUM_4;
    constexpr gpio_num_t VALVE_1_CONTROL_PIN_1 = GPIO_NUM_0;
    constexpr gpio_num_t VALVE_1_CONTROL_PIN_2 = GPIO_NUM_1;
    constexpr gpio_num_t VALVE_1_DRIVER_ENABLE_PIN = GPIO_NUM_2;

    constexpr gpio_num_t VALVE_2_CONTROL_PIN_1 = GPIO_NUM_3;
    constexpr gpio_num_t VALVE_2_CONTROL_PIN_2 = GPIO_NUM_4;
    constexpr gpio_num_t VALVE_2_DRIVER_ENABLE_PIN = GPIO_NUM_5;

    constexpr gpio_num_t VALVE_3_CONTROL_PIN_1 = GPIO_NUM_6;
    constexpr gpio_num_t VALVE_3_CONTROL_PIN_2 = GPIO_NUM_7;
    constexpr gpio_num_t VALVE_3_DRIVER_ENABLE_PIN = GPIO_NUM_8;

    constexpr gpio_num_t VALVE_4_CONTROL_PIN_1 = GPIO_NUM_9;
    constexpr gpio_num_t VALVE_4_CONTROL_PIN_2 = GPIO_NUM_10;
    constexpr gpio_num_t VALVE_4_DRIVER_ENABLE_PIN = GPIO_NUM_11;

    constexpr gpio_num_t VALVE_5_CONTROL_PIN_1 = GPIO_NUM_12;
    constexpr gpio_num_t VALVE_5_CONTROL_PIN_2 = GPIO_NUM_13;
    constexpr gpio_num_t VALVE_5_DRIVER_ENABLE_PIN = GPIO_NUM_14;

    constexpr gpio_num_t VALVE_6_CONTROL_PIN_1 = GPIO_NUM_15;
    constexpr gpio_num_t VALVE_6_CONTROL_PIN_2 = GPIO_NUM_16;
    constexpr gpio_num_t VALVE_6_DRIVER_ENABLE_PIN = GPIO_NUM_17;
}