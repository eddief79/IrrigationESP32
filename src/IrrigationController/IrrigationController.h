#pragma once
#include "Valve.h"
#include "StateBase.h"

enum class Action
{
    None,
    OpenValve,
    CloseValve,
    CloseAllValves
};

class IrrigationController
{
    private:
        StateBase *state_;
        Valve* valves_;

        void configure_valves();
    
    public:
        Action action_requested_;
        int action_valve_inndex_;

        IrrigationController(StateBase *state);
        void transition_to(StateBase *state);
        void do_work();
        void check_conditions();
        void request_action(Action action, int valve_index);
        ~IrrigationController();
};
