#include <stdio.h>
#include "IrrigationController.h"
#include "IdleState.h"

extern "C" void app_main()
{
    printf("Hello from ESP32-C6!\n");

    IrrigationController *controller = new IrrigationController(new IdleState);
    controller->do_work();
    controller->check_conditions();
}