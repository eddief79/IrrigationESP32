#pragma once
#include "IrrigationController/StateBase.h"

class OpeningValveState : public StateBase
{
    private:
        int valve_index_;
    public:
        OpeningValveState(int valve_index);
        void on_enter() override;
        void update() override;
};