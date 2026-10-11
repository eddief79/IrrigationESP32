#include "IrrigationController.h"
#include "driver/gpio.h"
#include <HardwareConfig.h>

IrrigationController::IrrigationController(FlowMeter& flowMeter, StateBase *state) : flowMeter_(flowMeter), state_(nullptr)
{
    valves_ = new Valve[6];
    configure_valves();
    transition_to(state);
}

void IrrigationController::transition_to(StateBase *state)
{
    //std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
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

void IrrigationController::start_watering(int valve_index)
{
    state_->start_watering(valve_index);
}

void IrrigationController::stop_watering(int valve_index)
{
    state_->stop_watering(valve_index);
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
