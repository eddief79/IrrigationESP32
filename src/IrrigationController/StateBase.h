#pragma once
#include "IrrigationController.h"

class StateBase 
{
    protected:
        IrrigationController *controller_;

    public:
        virtual ~StateBase() {}

  void StateBase::set_context(IrrigationController *controller)
  {
    this->controller_ = controller;
  }

  virtual void do_work() = 0;
  virtual void check_conditions() = 0;
};