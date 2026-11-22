#pragma once

#include <cstdint>
#include <esp_attr.h>
#include <soc/gpio_num.h>
#include <esp_err.h>

class FlowMeter
{
    public:
        FlowMeter();
        FlowMeter(gpio_num_t pin);
        static void IRAM_ATTR isrHandler(void* arg);
        uint64_t update();
        float getFlowRate() const;

    private:    
        gpio_num_t pin_;
        static constexpr uint64_t MILLILITERS_PER_LITER = 1000;
        static constexpr uint32_t FLOW_FILTER_SHIFT = 3;
        static constexpr uint64_t NO_FLOW_TIMEOUT_US = 500000;
        volatile uint64_t lastPulseTimeUs_ = 0;
        volatile uint32_t filteredPeriodUs_ = 0;
        volatile uint32_t pulseCount_ = 0;
        uint32_t lastPulseCount_ = 0;
        float flowRate_ = 0.0f;
        void configure_hardware();    
};