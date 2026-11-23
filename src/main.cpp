#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "HardwareConfig.h"
#include "FlowMeter.h"
#include "IrrigationController/IrrigationController.h"
#include "Zigbee/ZigbeeController.h"
#include "WaterUsage.h"

FlowMeter meter(HardwareConfig::FLOW_METER_PIN);
WaterUsage water_usage;
IrrigationController irrigation_controller(meter, water_usage);
ZigbeeController zigbee_controller(irrigation_controller);

extern "C" void app_main()
{    
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_IRAM));
    ESP_ERROR_CHECK(gpio_isr_handler_add(HardwareConfig::FLOW_METER_PIN, FlowMeter::isrHandler, &meter));

    irrigation_controller.close_all_valves();
    irrigation_controller.start();
    zigbee_controller.start();
}