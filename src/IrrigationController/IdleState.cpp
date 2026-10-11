#include "IdleState.h"
#include "ErrorState.h"
#include "OpeningValveState.h"

void IdleState::on_enter()
{
}

void IdleState::update()
{
    if (controller_->flowMeter_.getFlowRate() > 0.0f) //TODO: Add a threshold to avoid false positives from noise in the flow meter signal.
    {
        controller_->transition_to(new ErrorState("Water flow detected in Idle state. All valves should be closed."));
    }
}

void IdleState::start_watering(int valve_index)
{
    controller_->transition_to(new OpeningValveState(valve_index));
}

void IdleState::stop_watering(int valve_index)
{
    controller_->transition_to(new ErrorState("Stop watering was requested in the Idle state. Nothing to stop."));
}
