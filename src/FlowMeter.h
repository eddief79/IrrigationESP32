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

        void update();
        void resetSessionTotal();

        uint32_t getPulseCount() const;
        float getFlowRate() const;
        float getTotalLiters() const;

    private:    
        gpio_num_t pin_;
        static constexpr float PULSES_PER_LITER = 660.0f;
        volatile uint32_t pulseCount_ = 0;
        uint32_t lastPulseCount_ = 0;
        float flowRate_ = 0.0f;
        float totalLiters_ = 0.0f;
        float sessionTotalLiters_ = 0.0f;
        void configure_hardware();    
};