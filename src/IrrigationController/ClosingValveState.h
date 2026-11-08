#pragma once
#include "IrrigationController/StateBase.h"

class ClosingValveState : public StateBase
{
    private:
        int valve_index_;
    public:
        ClosingValveState(int valve_index);
        void on_enter() override;
        void update() override;
};