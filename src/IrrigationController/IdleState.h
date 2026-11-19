#pragma once
#include <stdint.h>
#include "IrrigationController/StateBase.h"

class IdleState : public StateBase
{
    private:
        uint8_t delay_;
    public:
        void on_enter() override;
        void update() override;
        bool process_request();
};