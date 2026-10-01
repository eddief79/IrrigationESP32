#include "OpeningValveState.h"
#include "ClosingValveState.h"
#include "WateringState.h"
#include "ErrorState.h"

OpeningValveState::OpeningValveState(int valve_index)
{
    this->valve_index_ = valve_index;
}

void OpeningValveState::on_enter()
{
    this->controller_->valves_[this->valve_index_].Open();
    this->controller_->transition_to(new WateringState(valve_index_));
}

void OpeningValveState::update()
{    
}

void OpeningValveState::start_watering(int valve_index)
{
    this->controller_->transition_to(new ErrorState("Start watering requested in the opening valve state. Only one valve can be open at a time."));
}

void OpeningValveState::stop_watering(int valve_index)
{
    if (valve_index == this->valve_index_)
    {
        this->controller_->transition_to(new ClosingValveState(valve_index_));
    }
    else
    {
        this->controller_->transition_to(new ErrorState("Stop watering was requested for a zone that is not on."));
    }
}
