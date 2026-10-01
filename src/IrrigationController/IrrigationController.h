#pragma once
#include "Valve.h"
#include "StateBase.h"

class IrrigationController
{
    private:
        StateBase *state_;
        
        void configure_valves();
        
        public:
        Valve* valves_;

        IrrigationController(StateBase *state);
        void transition_to(StateBase *state);
        void update();
        void check_conditions();
        void start_watering(int valve_index);
        void stop_watering(int valve_index);
        ~IrrigationController();
};
