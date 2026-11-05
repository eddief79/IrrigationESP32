#include "IrrigationController/ClosingValveState.h"
#include "IrrigationController/IdleState.h"
#include "IrrigationController/IrrigationController.h"

ClosingValveState::ClosingValveState(int valve_index)
{
    this->valve_index_ = valve_index;
}

void ClosingValveState::on_enter()
{
    this->controller_->valves_[this->valve_index_].close();
    this->controller_->transition_to(new IdleState());
}

void ClosingValveState::update()
{
}

void ClosingValveState::start_watering(int valve_index)
{
}

void ClosingValveState::stop_watering(int valve_index)
{
}
