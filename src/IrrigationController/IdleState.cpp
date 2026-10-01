#include "IdleState.h"
#include "ErrorState.h"
#include "OpeningValveState.h"

void IdleState::on_enter()
{
}

void IdleState::update()
{
    //TODO: If there is water flow in the Idle state try closing all valves, throw error.
}

void IdleState::start_watering(int valve_index)
{
    this->controller_->transition_to(new OpeningValveState(valve_index));
}

void IdleState::stop_watering(int valve_index)
{
    this->controller_->transition_to(new ErrorState("Stop watering was requested in the Idle state. Nothing to stop."));
}
