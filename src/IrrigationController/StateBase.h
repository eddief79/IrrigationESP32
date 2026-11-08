#pragma once

class IrrigationController;

class StateBase 
{
    protected:
        IrrigationController *controller_;

    public:
        virtual ~StateBase() {}

  void set_controller(IrrigationController *controller)
  {
    this->controller_ = controller;
  }

  virtual void on_enter() = 0;
  virtual void update() = 0;
};