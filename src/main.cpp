#include "freertos/FreeRTOS.h"
#include "IrrigationController/IrrigationController.h"
#include "FlowMeter.h"
#include "GeneralConfig.h"
#include "HardwareConfig.h"
#include <nvs_flash.h>
#include "Zigbee/ZigbeeController.h"

FlowMeter meter(HardwareConfig::FLOW_METER_PIN);
IrrigationController irrigation_controller(meter);
ZigbeeController zigbee_controller(irrigation_controller);

extern "C" void app_main()
{    
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_IRAM));
    ESP_ERROR_CHECK(gpio_isr_handler_add(HardwareConfig::FLOW_METER_PIN, FlowMeter::isrHandler, &meter));

    for (size_t valve_index = 0; valve_index < GeneralConfig::ZONE_COUNT; ++valve_index)
        irrigation_controller.valves_[valve_index].close();

    irrigation_controller.start();
    zigbee_controller.start();
}