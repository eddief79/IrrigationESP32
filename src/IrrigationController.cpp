#include "IrrigationController.h"

IrrigationController::IrrigationController(IrrigationControllerState *state) : state_(nullptr)
{
    this->TransitionTo(state);
}

void IrrigationController::TransitionTo(IrrigationControllerState *state)
{
    //std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
    if (this->state_ != nullptr)
      delete this->state_;
    this->state_ = state;
    this->state_->set_context(this);
}

void IrrigationController::DoWork()
{
    this->state_->DoWork();
}

IrrigationController::~IrrigationController()
{
    delete state_;
}
