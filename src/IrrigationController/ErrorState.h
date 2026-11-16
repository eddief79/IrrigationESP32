#pragma once
#include "IrrigationController/StateBase.h"
#include <string>

class ErrorState : public StateBase
{
    public:
        ErrorState(std::string error_message);
        void on_enter() override;
        void update() override;
};