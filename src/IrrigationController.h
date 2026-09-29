#pragma once
#include "Valve.h"
#include "IrrigationControllerState.h"

class IrrigationController
{
private:
    IrrigationControllerState *state_;
    Valve valves[6];
public:
    IrrigationController(IrrigationControllerState *state);
    void TransitionTo(IrrigationControllerState *state);
    void DoWork();
    ~IrrigationController();
};
