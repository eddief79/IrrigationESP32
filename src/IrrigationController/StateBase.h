#pragma once
#include "IrrigationController.h"

class StateBase 
{
    protected:
        IrrigationController *controller_;

    public:
        virtual ~StateBase() {}

  void StateBase::set_controller(IrrigationController *controller)
  {
    this->controller_ = controller;
  }

  virtual void on_enter() = 0;
  virtual void update() = 0;
  virtual void start_watering(int valve_index) = 0;
  virtual void stop_watering(int valve_index) = 0;
};