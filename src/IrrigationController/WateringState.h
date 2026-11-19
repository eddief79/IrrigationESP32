#pragma once
#include "IrrigationController/StateBase.h"

class WateringState : public StateBase
{
    private:
        int valve_index_;
        int delay_;
    public:
        WateringState(int valve_index);
        void on_enter() override;
        void update() override;
        bool process_request();
};