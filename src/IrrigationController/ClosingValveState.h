#pragma once
#include "StateBase.h"

class ClosingValveState : public StateBase
{
    private:
        int valve_index_;
    public:
        ClosingValveState(int valve_index);
        void on_enter() override;
        void update() override;
        void start_watering(int valve_index) override;
        void stop_watering(int valve_index) override;
};