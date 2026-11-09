#include "IrrigationController/IrrigationController.h"
#include "HardwareConfig.h"
#include "IrrigationController/StateBase.h"
#include "IrrigationController/IdleState.h"

IrrigationController::IrrigationController(FlowMeter& flowMeter)
    : state_(nullptr),
      flowMeter_(flowMeter),
      valves_(new Valve[6])
{
    configure_valves();
    transition_to(new IdleState());
}

void IrrigationController::transition_to(StateBase *state)
{
    if (state_ != nullptr)
      delete state_;
    state_ = state;
    state_->set_controller(this);
    state_->on_enter(); 
}

void IrrigationController::update()
{
    flowMeter_.update();
    state_->update();
}

void IrrigationController::request_start_watering(int zone)
{
    requestQueue_.push({RequestType::StartWatering, zone});
}

void IrrigationController::request_stop_watering(int zone)
{
    requestQueue_.push({RequestType::StopWatering, zone});
}

std::optional<IrrigationRequest> IrrigationController::get_next_request()
{
    if (requestQueue_.empty())
        return std::nullopt;

    IrrigationRequest request = requestQueue_.front();
    requestQueue_.pop();
    return request;
}

void IrrigationController::configure_valves()
{
    valves_[0] = Valve(Hardware::VALVE_1_CONTROL_PIN_1, Hardware::VALVE_1_CONTROL_PIN_2, Hardware::VALVE_1_DRIVER_ENABLE_PIN);
    valves_[1] = Valve(Hardware::VALVE_2_CONTROL_PIN_1, Hardware::VALVE_2_CONTROL_PIN_2, Hardware::VALVE_2_DRIVER_ENABLE_PIN);
    valves_[2] = Valve(Hardware::VALVE_3_CONTROL_PIN_1, Hardware::VALVE_3_CONTROL_PIN_2, Hardware::VALVE_3_DRIVER_ENABLE_PIN);
    valves_[3] = Valve(Hardware::VALVE_4_CONTROL_PIN_1, Hardware::VALVE_4_CONTROL_PIN_2, Hardware::VALVE_4_DRIVER_ENABLE_PIN);
    valves_[4] = Valve(Hardware::VALVE_5_CONTROL_PIN_1, Hardware::VALVE_5_CONTROL_PIN_2, Hardware::VALVE_5_DRIVER_ENABLE_PIN);
    valves_[5] = Valve(Hardware::VALVE_6_CONTROL_PIN_1, Hardware::VALVE_6_CONTROL_PIN_2, Hardware::VALVE_6_DRIVER_ENABLE_PIN);
}

IrrigationController::~IrrigationController()
{
    delete state_;
    delete[] valves_;
}
