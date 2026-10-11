#include "FlowMeter.h"

FlowMeter::FlowMeter()
{
}

FlowMeter::FlowMeter(gpio_num_t pin)
{
    this->pin_ = pin;
}

void FlowMeter::update()
{
    lastPulseCount_ = pulseCount_;

    uint32_t pulses = pulseCount_ - lastPulseCount_;
    lastPulseCount_ = pulseCount_;

    // Convert pulses to volume
    float liters = pulses / PULSES_PER_LITER;

    total_liters += liters;

    // Since this was a 1-second interval:
    flow_lpm = liters * 60.0f;
}

void IRAM_ATTR FlowMeter::isrHandler(void *arg)
{
    auto* meter = static_cast<FlowMeter*>(arg);
    ++meter->pulseCount_;
}
