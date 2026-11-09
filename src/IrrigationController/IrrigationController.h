#pragma once
#include <queue>
#include "Valve.h"
#include "FlowMeter.h"
#include "IrrigationController/IrrigationRequest.h"

class StateBase;

class IrrigationController
{
    private:        
        StateBase *state_;
        void configure_valves();
        std::queue<IrrigationRequest> requestQueue_;
        
    public:
        FlowMeter& flowMeter_;
        Valve* valves_;
        IrrigationController(FlowMeter& flowMeter);
        void transition_to(StateBase *state);
        void update();
        void request_start_watering(int zone);
        void request_stop_watering(int zone);
        std::optional<IrrigationRequest> get_next_request();
        ~IrrigationController();
};
