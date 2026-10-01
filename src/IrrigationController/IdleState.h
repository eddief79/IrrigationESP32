#pragma once
#include "StateBase.h"

class IdleState : public StateBase
{
    public:
    void on_enter() override;
    void update() override;
    void start_watering(int valve_index) override;
    void stop_watering(int valve_index) override;
};