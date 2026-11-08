#pragma once
#include <string>
#include "IrrigationController/StateBase.h"

class ErrorState : public StateBase
{
    public:
        ErrorState(std::string error_message);
        void on_enter() override;
        void update() override;
};