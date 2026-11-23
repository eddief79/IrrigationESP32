#pragma once
#include "ezbee/af.h"
#include "ezbee/zha.h"
#include "ezbee/zcl/cluster/basic_desc.h"
#include "ezbee/zcl/cluster/identify_desc.h"
#include "ezbee/zcl/cluster/on_off_desc.h"
#include "ezbee/zcl/zcl_common.h"
#include "esp_zigbee.h"
#include "IrrigationController/IrrigationEventListener.h"

class IrrigationController;

class ZigbeeController : public IrrigationEventListener
{
    private:
        static ZigbeeController* instance_;
        IrrigationController& irigationController_;
        TaskHandle_t taskHandle_;   
        TimerHandle_t steering_retry_timer = nullptr;

        static void task_entry(void *pvParameters);
        static void retry_network_steering(TimerHandle_t);
        static bool zigbee_app_signal_handler(const ezb_app_signal_t* signal);
        static void zigbee_action_handler(ezb_zcl_core_action_callback_id_t callback_id, void* message);
        static void set_zigbee_zone_state(uint8_t zone, bool on);
        static void set_zigbee_zone_cycle_volume(uint8_t zone, float liters);
        static void set_zigbee_error_code(uint8_t error_code, uint8_t zone);
        void initialize_zigbee();
        bool handle_zigbee_app_signal(const ezb_app_signal_t* signal);
        esp_err_t create_zone_endpoints();
        void handle_on_off_attribute(ezb_zcl_set_attr_value_message_t* message);

    public:
        ZigbeeController(IrrigationController& controller);
        void start();
        void on_request_rejected(const IrrigationRequest& request) override;
        void on_error(uint8_t error_code, uint8_t zone) override;
        void on_watering_started(uint8_t zone) override;
        void on_watering_completed(uint8_t zone, float liters) override;
        void on_measurements_updated() override;
};