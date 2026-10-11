#pragma once
#include <string>
#include "StateBase.h"

class ErrorState : public StateBase
{
    public:
        ErrorState(std::string error_message);
        void on_enter() override;
        void update() override;
        void start_watering(int valve_index) override;
        void stop_watering(int valve_index) override;
};