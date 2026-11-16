#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "Zigbee/ZigbeeController.h"
#include "IrrigationController/IrrigationController.h"

constexpr char TAG[] = "IRRIGATION_ZIGBEE";
constexpr uint8_t FIRST_ENDPOINT = 1;
constexpr uint8_t ZONE_COUNT = 6;
constexpr uint32_t ALL_ZIGBEE_CHANNELS = 0x07FFF800U;
constexpr char MANUFACTURER[] = "\x0f" "Open Irrigation";
constexpr char MODEL[] = "\x0f" "IrrigationESP32";

void ZigbeeController::task_entry(void *pvParameters)
{
    auto *controller = static_cast<ZigbeeController *>(pvParameters);
    controller->initialize_zigbee();
}

void ZigbeeController::retry_network_steering(TimerHandle_t timer)
{
    esp_zigbee_lock_acquire(portMAX_DELAY);
    ezb_bdb_start_top_level_commissioning(EZB_BDB_MODE_NETWORK_STEERING);
    esp_zigbee_lock_release();
}

void ZigbeeController::initialize_zigbee()
{
    esp_zigbee_config_t config = {
        .device_config = {
            .device_type = EZB_NWK_DEVICE_TYPE_END_DEVICE,
            .install_code_policy = false,
            .zed_config = {
                .ed_timeout = EZB_NWK_ED_TIMEOUT_64MIN,
                .keep_alive = 3000,
            },
        },
        .platform_config = {
            .storage_partition_name = "nvs",
            .radio_config = {
                ESP_ZIGBEE_RADIO_MODE_NATIVE,
                {},
            },
        },
    };

    ESP_ERROR_CHECK(esp_zigbee_init(&config));    
    steering_retry_timer = xTimerCreate("zb_steer_retry", pdMS_TO_TICKS(1000), pdFALSE, this, retry_network_steering);
    configASSERT(steering_retry_timer != nullptr);
    ESP_ERROR_CHECK(ezb_bdb_set_primary_channel_set(ALL_ZIGBEE_CHANNELS));
    ESP_ERROR_CHECK(ezb_app_signal_add_handler(zigbee_app_signal_handler));
    ESP_ERROR_CHECK(create_zone_endpoints());
    ESP_ERROR_CHECK(esp_zigbee_start(false));
    esp_zigbee_launch_mainloop();
    vTaskDelete(taskHandle_);
}

bool ZigbeeController::zigbee_app_signal_handler(const ezb_app_signal_t *signal)
{
    return instance_ ? instance_->handle_zigbee_app_signal(signal) : true;
}

void ZigbeeController::zigbee_action_handler(ezb_zcl_core_action_callback_id_t callback_id, void *message)
{
    if (instance_ && callback_id == EZB_ZCL_CORE_SET_ATTR_VALUE_CB_ID)
        instance_->handle_on_off_attribute(static_cast<ezb_zcl_set_attr_value_message_t*>(message));
}

void ZigbeeController::set_zigbee_zone_state(uint8_t zone, bool on)
{
    if (zone >= ZONE_COUNT)
        return;

    const uint8_t endpoint = FIRST_ENDPOINT + zone;
    uint8_t value = on ? 1 : 0;
    esp_zigbee_lock_acquire(portMAX_DELAY);
    const ezb_zcl_status_t status = ezb_zcl_set_attr_value(
        endpoint,
        EZB_ZCL_CLUSTER_ID_ON_OFF,
        EZB_ZCL_CLUSTER_SERVER,
        EZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,
        EZB_ZCL_STD_MANUF_CODE,
        &value,
        false);
    esp_zigbee_lock_release();

    if (status != EZB_ZCL_STATUS_SUCCESS)
        ESP_LOGW(TAG, "Failed updating zone %u state (status 0x%02x)", zone + 1, status);
}

bool ZigbeeController::handle_zigbee_app_signal(const ezb_app_signal_t *signal)
{
    const ezb_app_signal_type_t signal_type = ezb_app_signal_get_type(signal);

    switch (signal_type)
    {
        case EZB_ZDO_SIGNAL_SKIP_STARTUP:
            ESP_LOGI(TAG, "Initializing Zigbee stack");
            ezb_bdb_start_top_level_commissioning(EZB_BDB_MODE_INITIALIZATION);
            break;

        case EZB_BDB_SIGNAL_DEVICE_FIRST_START:
        case EZB_BDB_SIGNAL_DEVICE_REBOOT:
        {
            const auto status = *static_cast<const ezb_bdb_comm_status_t*>(ezb_app_signal_get_params(signal));
            if (status == EZB_BDB_STATUS_SUCCESS && ezb_bdb_is_factory_new())
            {
                ESP_LOGI(TAG, "Joining the Zigbee2MQTT network");
                ezb_bdb_start_top_level_commissioning(EZB_BDB_MODE_NETWORK_STEERING);
            }
            break;
        }

        case EZB_BDB_SIGNAL_STEERING:
        {
            const auto status = *static_cast<const ezb_bdb_comm_status_t*>(ezb_app_signal_get_params(signal));
            if (status == EZB_BDB_STATUS_SUCCESS)
                ESP_LOGI(TAG, "Joined Zigbee network");
            else
            {
                ESP_LOGW(TAG, "Network steering failed with status 0x%02x", status);
                xTimerStart(steering_retry_timer, 0);
            }
            break;
        }

        default:
            break;
    }

    return true;
}

esp_err_t ZigbeeController::create_zone_endpoints()
{
    ezb_af_device_desc_t device = ezb_af_create_device_desc();
    
    if (device == nullptr)
        return ESP_FAIL;

    for (uint8_t zone = 0; zone < ZONE_COUNT; ++zone)
    {
        ezb_zcl_basic_cluster_server_config_t basic_config = {
            .zcl_version = EZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE,
            .power_source = EZB_ZCL_BASIC_POWER_SOURCE_DEFAULT_VALUE,
        };

        ezb_zcl_identify_cluster_server_config_t identify_config = {
            .identify_time = EZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE,
        };

        ezb_zcl_on_off_cluster_server_config_t on_off_config = {.on_off = false};

        ezb_zcl_cluster_desc_t basic = ezb_zcl_basic_create_cluster_desc(&basic_config, EZB_ZCL_CLUSTER_SERVER);
        ezb_zcl_cluster_desc_t identify = ezb_zcl_identify_create_cluster_desc(&identify_config, EZB_ZCL_CLUSTER_SERVER);
        ezb_zcl_cluster_desc_t on_off = ezb_zcl_on_off_create_cluster_desc(&on_off_config, EZB_ZCL_CLUSTER_SERVER);

        if (basic == nullptr || identify == nullptr || on_off == nullptr)
            return ESP_FAIL;

        ESP_ERROR_CHECK(ezb_zcl_basic_cluster_desc_add_attr(basic, EZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID, MANUFACTURER));
        ESP_ERROR_CHECK(ezb_zcl_basic_cluster_desc_add_attr(basic, EZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID, MODEL));

        const uint8_t endpoint_id = FIRST_ENDPOINT + zone;
        const ezb_af_ep_config_t endpoint_config = {
            .ep_id = endpoint_id,
            .app_profile_id = EZB_AF_HA_PROFILE_ID,
            .app_device_id = EZB_ZHA_ON_OFF_OUTPUT_DEVICE_ID,
            .app_device_version = 0,
            .reserved = {},
        };

        ezb_af_ep_desc_t endpoint = ezb_af_create_endpoint_desc(&endpoint_config);
        
        if (endpoint == nullptr)
            return ESP_FAIL;

        ESP_ERROR_CHECK(ezb_af_endpoint_add_cluster_desc(endpoint, basic));
        ESP_ERROR_CHECK(ezb_af_endpoint_add_cluster_desc(endpoint, identify));
        ESP_ERROR_CHECK(ezb_af_endpoint_add_cluster_desc(endpoint, on_off));
        ESP_ERROR_CHECK(ezb_af_device_add_endpoint_desc(device, endpoint));
    }

    ESP_ERROR_CHECK(ezb_af_device_desc_register(device));
    ezb_zcl_core_action_handler_register(zigbee_action_handler);
    return ESP_OK;
}

void ZigbeeController::handle_on_off_attribute(ezb_zcl_set_attr_value_message_t *message)
{
        if (message == nullptr ||
        message->info.cluster_id != EZB_ZCL_CLUSTER_ID_ON_OFF ||
        message->in.attribute.id != EZB_ZCL_ATTR_ON_OFF_ON_OFF_ID ||
        message->info.status != EZB_ZCL_STATUS_SUCCESS ||
        message->in.attribute.data.value == nullptr)
    {
        return;
    }

    const uint8_t endpoint = message->info.dst_ep;
    if (endpoint < FIRST_ENDPOINT || endpoint >= FIRST_ENDPOINT + ZONE_COUNT)
        return;

    bool startWaterinng = *static_cast<const uint8_t*>(message->in.attribute.data.value) != 0;
    uint8_t zone = static_cast<uint8_t>(endpoint - FIRST_ENDPOINT);
    bool result;

    if (startWaterinng)
        result = irigationController_.request_start_watering(zone);
    else
        result = irigationController_.request_stop_watering(zone);
    
    if (!result)
        ESP_LOGE(TAG, "Dropping zone %u command: irrigation queue is full", zone);
}

ZigbeeController* ZigbeeController::instance_ = nullptr;

ZigbeeController::ZigbeeController(IrrigationController &controller)
    : irigationController_(controller)
{
    instance_ = this;
}

void ZigbeeController::start()
{
    BaseType_t result = xTaskCreate(task_entry, "zigbee", 8192, this, 5, &taskHandle_);
    configASSERT(result == pdPASS);
}
