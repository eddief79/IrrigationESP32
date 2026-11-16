#include "IrrigationController/ClosingValveState.h"
#include "IrrigationController/ErrorState.h"
#include "IrrigationController/IdleState.h"
#include "IrrigationController/IrrigationController.h"
#include "IrrigationController/OpeningValveState.h"

ClosingValveState::ClosingValveState(int valve_index)
{
    this->valve_index_ = valve_index;
}

void ClosingValveState::on_enter()
{
    this->controller_->valves_[this->valve_index_].close();
    this->controller_->eventListener_->on_watering_completed(this->valve_index_, 1.0f); 
    //TODO: Replace 1.0f with the actual amount of water that was dispensed during the watering session.
    this->controller_->transition_to(new IdleState());
}

void ClosingValveState::update()
{
}
