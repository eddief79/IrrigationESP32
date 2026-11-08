#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "IrrigationController/IrrigationController.h"
#include "FlowMeter.h"
#include "HardwareConfig.h"
#include <esp_log.h>

FlowMeter meter(Hardware::FLOW_METER_PIN);
IrrigationController controller(meter);
    
// void zigbee_task(void *arg) 
// { 
//     for (;;) 
//     {

//         vTaskDelay(pdMS_TO_TICKS(500));
//     }
// }

// void irrigation_task(void *arg) 
// {
    
//     for (;;) 
//     {
//         auto& controller = *static_cast<IrrigationController*>(arg);
//         controller.update();
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

extern "C" void app_main()
{    
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_IRAM));
    gpio_isr_handler_add(Hardware::FLOW_METER_PIN, FlowMeter::isrHandler, &meter);
    // xTaskCreate(irrigation_task, "irrigation", 2048, &controller, 3, NULL);
    // xTaskCreate(zigbee_task, "zigbee", 2048, NULL, 3, NULL);

    while(1)
    {
        meter.update();
        ESP_LOGI("IRRIGATION", "Flow rate: %f L/min; Total liters: %f", meter.getFlowRate(), meter.getTotalLiters());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}