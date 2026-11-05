#pragma once
#include "Valve.h"
#include "FlowMeter.h"

class StateBase;

class IrrigationController
{
    private:        
    StateBase *state_;
    void configure_valves();
    
    public:
        FlowMeter& flowMeter_;
        Valve* valves_;
        IrrigationController(StateBase *state, FlowMeter& flowMeter);
        void transition_to(StateBase *state);
        void update();
        void start_watering(int valve_index);
        void stop_watering(int valve_index);
        ~IrrigationController();
};
