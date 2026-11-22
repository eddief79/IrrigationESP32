#include "IrrigationController/ClosingValveState.h"
#include "IrrigationController/ErrorState.h"
#include "WaterUsage.h"
#include "IrrigationController/IdleState.h"
#include "IrrigationController/IrrigationController.h"
#include "IrrigationController/OpeningValveState.h"

ClosingValveState::ClosingValveState(int valve_index)
{
    valve_index_ = valve_index;
}

void ClosingValveState::on_enter()
{
    controller_->valves_[valve_index_].close();
    controller_->get_water_usage().save_totals();
    controller_->eventListener_->on_watering_completed(
        valve_index_, 
        controller_->get_water_usage().get_current_cycle_total_l());
    controller_->get_water_usage().reset_current_cycle_total();
    controller_->transition_to(new IdleState());
}

void ClosingValveState::update()
{
}
