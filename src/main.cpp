#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "IrrigationController.h"
#include "IdleState.h"

void irrigation_task(void *arg) 
{
    IrrigationController *controller = new IrrigationController(new IdleState);
    
    for (;;) 
    {
        controller->do_work();
        controller->check_conditions();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

extern "C" void app_main()
{
    printf("Hello from ESP32-C6!\n");

    xTaskCreate(irrigation_task, "irrigation", 2048, NULL, 3, NULL);
}