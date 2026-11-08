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
    this->controller_->transition_to(new IdleState());
}

void ClosingValveState::update()
{
}
