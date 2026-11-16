#pragma once
#include <queue>
#include "Valve.h"
#include "FlowMeter.h"
#include "IrrigationController/IrrigationRequest.h"
#include "IrrigationController/IrrigationEventListener.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

class StateBase;

class IrrigationController
{
    private:        
        StateBase *state_;
        void configure_valves();
        QueueHandle_t requestQueue_;
        
        public:
        FlowMeter& flowMeter_;
        Valve* valves_;
        IrrigationEventListener* eventListener_ = nullptr;
        IrrigationController(FlowMeter& flowMeter);
        void start();
        void transition_to(StateBase *state);
        void update();
        bool request_start_watering(uint8_t  zone);
        bool request_stop_watering(uint8_t  zone);
        std::optional<IrrigationRequest> get_next_request();
        void set_event_listener(IrrigationEventListener* listener);
        ~IrrigationController();
};
