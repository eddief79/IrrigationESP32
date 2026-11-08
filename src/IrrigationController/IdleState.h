#pragma once
#include "IrrigationController/StateBase.h"

class IdleState : public StateBase
{
    private:
        static constexpr int FLOW_CHECK_DELAY_SECONDS = 3;
        int delay_;
    public:
        void on_enter() override;
        void update() override;
        void process_request();
};