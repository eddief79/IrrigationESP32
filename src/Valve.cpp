#include "Valve.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

Valve::Valve()
{
}

Valve::Valve(gpio_num_t control_pin_1, gpio_num_t control_pin_2, gpio_num_t driver_enable_pin)
{
    this->control_pin_1_ = control_pin_1;
    this->control_pin_2_ = control_pin_2;
    this->driver_enable_pin_ = driver_enable_pin;
}

void Valve::Open()
{
    gpio_set_level(driver_enable_pin_, 1);    
    vTaskDelay(pdMS_TO_TICKS(2)); //wait 2-3ms
    gpio_set_level(control_pin_1_, 1);
    gpio_set_level(control_pin_2_, 0);    
    vTaskDelay(pdMS_TO_TICKS(50)); //wait 50ms
    gpio_set_level(control_pin_1_, 0);
    gpio_set_level(control_pin_2_, 0);
    gpio_set_level(driver_enable_pin_, 0);
}

void Valve::Close()
{
    gpio_set_level(driver_enable_pin_, 1);
    vTaskDelay(pdMS_TO_TICKS(2)); //wait 2-3ms
    gpio_set_level(control_pin_1_, 0);
    gpio_set_level(control_pin_2_, 1);
    vTaskDelay(pdMS_TO_TICKS(50)); //wait 50ms
    gpio_set_level(control_pin_1_, 0);
    gpio_set_level(control_pin_2_, 0);
    gpio_set_level(driver_enable_pin_, 0);
}
