#pragma once
#include <cstdint>
#include <queue>
#include "Valve.h"
#include "IrrigationController/IrrigationRequest.h"
#include "IrrigationController/IrrigationEventListener.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

class StateBase;
class FlowMeter;
class WaterUsage;

class IrrigationController
{
    private:        
        FlowMeter& flowMeter_;
        WaterUsage& waterUsage_;
        StateBase *state_;
        QueueHandle_t requestQueue_;
        uint64_t waterUsedDelta_ml_ = 0;
        int64_t wateringStartTimeUs_ = 0;
        void configure_valves();
        
    public:
        IrrigationEventListener* eventListener_ = nullptr;
        Valve* valves_;
        IrrigationController(FlowMeter& flowMeter, WaterUsage& waterUsage);
        void start();
        void transition_to(StateBase *state);
        void update();
        bool request_start_watering(uint8_t  zone);
        bool request_stop_watering(uint8_t  zone);
        std::optional<IrrigationRequest> get_next_request();
        void set_event_listener(IrrigationEventListener* listener);
        FlowMeter& get_flow_meter();
        WaterUsage& get_water_usage();
        uint64_t get_water_used_delta();
        void start_watering_timer();
        uint64_t get_watering_elapsed_seconds() const;
        ~IrrigationController();
};
