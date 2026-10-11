#pragma once
#include "Valve.h"
#include "StateBase.h"
#include <FlowMeter.h>

class IrrigationController
{
    private:        
        FlowMeter& flowMeter_;
        StateBase *state_;
        void configure_valves();
        
        public:
        Valve* valves_;
        IrrigationController(FlowMeter& flowMeter, StateBase *state);
        void transition_to(StateBase *state);
        void update();
        void start_watering(int valve_index);
        void stop_watering(int valve_index);
        ~IrrigationController();
};
