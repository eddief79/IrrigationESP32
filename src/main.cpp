#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "IrrigationController.h"
#include "IdleState.h"
#include "FlowMeter.h"

FlowMeter meter(GPIO_NUM_4);
IrrigationController controller(meter, new IdleState());

void zigbee_task(void *arg) 
{ 
    for (;;) 
    {

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void irrigation_task(void *arg) 
{
    
    for (;;) 
    {
        auto& controller = *static_cast<IrrigationController*>(arg);
        controller.update();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void flow_meter_task(void *arg) 
{
    auto& meter = *static_cast<FlowMeter*>(arg);
    gpio_isr_handler_add(GPIO_NUM_4, FlowMeter::isrHandler, &meter);
    
    for (;;) 
    {
        meter.update();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C" void app_main()
{
    printf("Hello from ESP32-C6!\n");

    xTaskCreate(irrigation_task, "irrigation", 2048, &controller, 3, NULL);
    xTaskCreate(zigbee_task, "zigbee", 2048, NULL, 3, NULL);
}