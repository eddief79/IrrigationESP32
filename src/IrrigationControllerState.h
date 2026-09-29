#pragma once
#include "IrrigationController.h"

class IrrigationControllerState 
{
    protected:
        IrrigationController *context_;

    public:
        virtual ~IrrigationControllerState() {}

  void IrrigationControllerState::set_context(IrrigationController *context);

  virtual void DoWork() = 0;
};