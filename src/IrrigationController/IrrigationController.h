#pragma once
#include "Valve.h"
#include "StateBase.h"

class IrrigationController
{
    private:
        StateBase *state_;
        Valve* valves_;
        void configure_valves();
    public:
        IrrigationController(StateBase *state);
        void transition_to(StateBase *state);
        void do_work();
        void check_conditions();
        ~IrrigationController();
};
