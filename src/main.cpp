#include "freertos/FreeRTOS.h"
#include "IrrigationController/IrrigationController.h"
#include "FlowMeter.h"
#include "HardwareConfig.h"
#include <nvs_flash.h>
#include "Zigbee/ZigbeeController.h"

FlowMeter meter(Hardware::FLOW_METER_PIN);
IrrigationController irrigation_controller(meter);
ZigbeeController zigbee_controller(irrigation_controller);

extern "C" void app_main()
{    
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_IRAM));
    ESP_ERROR_CHECK(gpio_isr_handler_add(Hardware::FLOW_METER_PIN, FlowMeter::isrHandler, &meter));

    for (size_t valve_index = 0; valve_index < 6; ++valve_index)
        irrigation_controller.valves_[valve_index].close();

    irrigation_controller.start();
    zigbee_controller.start();
//crete zigbee task
    
    // xTaskCreate(zigbee_task, "zigbee", 4096, NULL, 3, NULL);

    // while(1)
    // {
    //     for (size_t i = 0; i < 6; ++i)
    //     {
    //         for (int j = 0; j < 10; ++j)
    //         {
    //             ESP_LOGI("IRRIGATION", "Opening valve %d", i);
    //             controller.request_start_watering(i);
    //             vTaskDelay(pdMS_TO_TICKS(3000));
    //             ESP_LOGI("IRRIGATION", "Closing valve %d", i);
    //             controller.request_stop_watering(i);
    //             vTaskDelay(pdMS_TO_TICKS(3000));
    //         }
    //     }        
    //     //meter.update();
    //     //ESP_LOGI("IRRIGATION", "Flow rate: %f L/min; Total liters: %f", meter.getFlowRate(), meter.getTotalLiters());
    //     //vTaskDelay(pdMS_TO_TICKS(1000));
    // }
}