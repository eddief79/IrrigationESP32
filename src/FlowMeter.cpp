#include "FlowMeter.h"
#include <driver/gpio.h>

FlowMeter::FlowMeter()
{
}

FlowMeter::FlowMeter(gpio_num_t pin)
{
    pin_ = pin;
    configure_hardware();
}

void FlowMeter::update()
{
    uint32_t pulses = pulseCount_ - lastPulseCount_;
    lastPulseCount_ = pulseCount_;

    // Convert pulses to volume
    float liters = pulses / PULSES_PER_LITER;

    totalLiters_ += liters;
    sessionTotalLiters_ += liters;

    // Since this was a 1-second interval:
    flowRate_ = liters * 60.0f;
}

void FlowMeter::resetSessionTotal()
{
    sessionTotalLiters_ = 0.0f;
}

float FlowMeter::getFlowRate() const
{
    return flowRate_;
}

float FlowMeter::getTotalLiters() const
{
    return totalLiters_;
}

void FlowMeter::configure_hardware()
{
    gpio_config_t flowMeterConfig = {};
    flowMeterConfig.pin_bit_mask = 1ULL << pin_;
    flowMeterConfig.mode = GPIO_MODE_INPUT;
    flowMeterConfig.pull_up_en = GPIO_PULLUP_ENABLE;
    flowMeterConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    flowMeterConfig.intr_type = GPIO_INTR_POSEDGE;

    ESP_ERROR_CHECK(gpio_config(&flowMeterConfig));
}

void IRAM_ATTR FlowMeter::isrHandler(void *arg)
{
    auto* meter = static_cast<FlowMeter*>(arg);
    ++meter->pulseCount_;
}
