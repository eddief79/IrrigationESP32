#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "IrrigationController.h"
#include "IdleState.h"
#include "FlowMeter.h"
#include "HardwareConfig.h"

FlowMeter meter(Hardware::FLOW_METER_PIN);
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

extern "C" void app_main()
{
    printf("Hello from ESP32-C6!\n");
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_IRAM));
    xTaskCreate(irrigation_task, "irrigation", 2048, &controller, 3, NULL);
    xTaskCreate(zigbee_task, "zigbee", 2048, NULL, 3, NULL);
}