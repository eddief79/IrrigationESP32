#pragma once
#include "IrrigationController/StateBase.h"

class WateringState : public StateBase
{
    private:
        static constexpr int FLOW_CHECK_DELAY_SECONDS = 3;
        int valve_index_;
        int delay_;
    public:
        WateringState(int valve_index);
        void on_enter() override;
        void update() override;
        void process_request();
};