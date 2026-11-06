#include "FlowMeter.h"
#include <driver/gpio.h>
#include <esp_log.h>
#include <esp_timer.h>

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
    // ---------------------------------------------------------
    // Volume calculation
    // ---------------------------------------------------------

    uint32_t pulses = pulseCount_ - lastPulseCount_;
    lastPulseCount_ = pulseCount_;

    float liters = static_cast<float>(pulses) / PULSES_PER_LITER;

    totalLiters_ += liters;
    sessionTotalLiters_ += liters;


    // ---------------------------------------------------------
    // Instantaneous flow calculation
    // ---------------------------------------------------------

    uint64_t lastPulseTimeUs = lastPulseTimeUs_;
    uint32_t filteredPeriodUs = filteredPeriodUs_;

    if (lastPulseTimeUs == 0 || filteredPeriodUs == 0)
    {
        flowRate_ = 0.0f;
        return;
    }

    uint64_t nowUs = esp_timer_get_time();

    if ((nowUs - lastPulseTimeUs) > NO_FLOW_TIMEOUT_US)
    {
        flowRate_ = 0.0f;
    }
    else
    {
        // Q = 60,000,000 / (pulses_per_liter × period_us)
        flowRate_ = 60000000.0f / (PULSES_PER_LITER * static_cast<float>(filteredPeriodUs));
    }
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
    uint64_t nowUs = esp_timer_get_time();
    meter->pulseCount_ = meter->pulseCount_ + 1u;

    if (meter->lastPulseTimeUs_ != 0)
    {
        uint32_t periodUs =
            static_cast<uint32_t>(
                nowUs - meter->lastPulseTimeUs_
            );

        if (meter->filteredPeriodUs_ == 0)
        {
            // First valid period establishes the initial value.
            meter->filteredPeriodUs_ = periodUs;
        }
        else
        {
            // Exponential moving average:
            //
            // filtered += (new - filtered) / 8
            //
            int32_t error =
                static_cast<int32_t>(periodUs) -
                static_cast<int32_t>(meter->filteredPeriodUs_);

            meter->filteredPeriodUs_ +=
                error >> FLOW_FILTER_SHIFT;
        }
    }

    meter->lastPulseTimeUs_ = nowUs;
}

