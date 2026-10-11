#include "Valve.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

Valve::Valve()
{
}

Valve::Valve(gpio_num_t control_pin_1, gpio_num_t control_pin_2, gpio_num_t driver_enable_pin)
{
    control_pin_1_ = control_pin_1;
    control_pin_2_ = control_pin_2;
    driver_enable_pin_ = driver_enable_pin;
    configure_hardware();
}

void Valve::open()
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

void Valve::close()
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

void Valve::configure_hardware()
{
    gpio_config_t valveConfig = {};
    valveConfig.pin_bit_mask = 1ULL << control_pin_1_ | 1ULL << control_pin_2_ | 1ULL << driver_enable_pin_;
    valveConfig.mode = GPIO_MODE_OUTPUT;
    valveConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    valveConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    valveConfig.intr_type = GPIO_INTR_DISABLE;

    ESP_ERROR_CHECK(gpio_config(&valveConfig));
}
