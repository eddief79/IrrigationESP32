#pragma once
#include "IrrigationController.h"

class StateBase 
{
    protected:
        IrrigationController *context_;

    public:
        virtual ~StateBase() {}

  void StateBase::set_context(IrrigationController *context)
  {
    this->context_ = context;
  }

  virtual void do_work() = 0;
  virtual void check_conditions() = 0;
};