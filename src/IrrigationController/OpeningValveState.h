#pragma once
#include "StateBase.h"

class OpeningValveState : public StateBase
{
    private:
    int valve_index_;
    public:
    OpeningValveState(int valve_index);
    void on_enter() override;
    void update() override;
    void start_watering(int valve_index) override;
    void stop_watering(int valve_index) override;
};