#include "IrrigationController/OpeningValveState.h"
#include "IrrigationController/ClosingValveState.h"
#include "IrrigationController/WateringState.h"
#include "IrrigationController/ErrorState.h"
#include "IrrigationController/IrrigationController.h"

OpeningValveState::OpeningValveState(int valve_index)
{
    this->valve_index_ = valve_index;
}

void OpeningValveState::on_enter()
{
    controller_->flowMeter_.resetSessionTotal();
    controller_->valves_[valve_index_].open();
    controller_->transition_to(new WateringState(valve_index_));
}

void OpeningValveState::update()
{    
}

