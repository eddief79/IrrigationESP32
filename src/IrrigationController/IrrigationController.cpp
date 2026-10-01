#include "IrrigationController.h"
#include "driver/gpio.h"

IrrigationController::IrrigationController(StateBase *state) : state_(nullptr)
{
    valves_ = new Valve[6];
    configure_valves();
    this->transition_to(state);
}

void IrrigationController::transition_to(StateBase *state)
{
    //std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
    if (this->state_ != nullptr)
      delete this->state_;
    this->state_ = state;
    this->state_->set_controller(this);
    this->state_->on_enter(); 
}

void IrrigationController::update()
{
    this->state_->update();
}

void IrrigationController::start_watering(int valve_index)
{
    this->state_->start_watering(valve_index);
}

void IrrigationController::stop_watering(int valve_index)
{
    this->state_->stop_watering(valve_index);
}

void IrrigationController::configure_valves()
{
    valves_[0] = Valve(GPIO_NUM_0, GPIO_NUM_1, GPIO_NUM_2);
    valves_[1] = Valve(GPIO_NUM_0, GPIO_NUM_1, GPIO_NUM_2);
    valves_[2] = Valve(GPIO_NUM_0, GPIO_NUM_1, GPIO_NUM_2);
    valves_[3] = Valve(GPIO_NUM_0, GPIO_NUM_1, GPIO_NUM_2);
    valves_[4] = Valve(GPIO_NUM_0, GPIO_NUM_1, GPIO_NUM_2);
    valves_[5] = Valve(GPIO_NUM_0, GPIO_NUM_1, GPIO_NUM_2);
}

IrrigationController::~IrrigationController()
{
    delete state_;
    delete[] valves_;
}
