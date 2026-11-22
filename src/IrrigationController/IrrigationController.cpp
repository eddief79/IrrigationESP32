#include "GeneralConfig.h"
#include "HardwareConfig.h"
#include "IrrigationController/IrrigationController.h"
#include "IrrigationController/StateBase.h"
#include "IrrigationController/IdleState.h"
#include "IrrigationController/IrrigationEventListener.h"
#include <FlowMeter.h>
#include <WaterUsage.h>

IrrigationController::IrrigationController(FlowMeter& flowMeter, WaterUsage& waterUsage)
    : flowMeter_(flowMeter),
      waterUsage_(waterUsage),
      state_(nullptr),
      valves_(new Valve[GeneralConfig::ZONE_COUNT])
{
    requestQueue_ = xQueueCreate(10, sizeof(IrrigationRequest));
    configASSERT(requestQueue_ != nullptr);
    configure_valves();
    waterUsage_.load_totals();
    transition_to(new IdleState());
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

void IrrigationController::start()
{
    BaseType_t task_result = xTaskCreate(irrigation_task, "irrigation", 4096, this, 3, nullptr);
    configASSERT(task_result == pdPASS);
}

void IrrigationController::transition_to(StateBase *state)
{
    if (state_ != nullptr)
      delete state_;
    state_ = state;
    state_->set_controller(this);
    state_->on_enter(); 
}

void IrrigationController::update()
{
    waterUsedDelta_ml_ = flowMeter_.update();
    state_->update();
}

bool IrrigationController::request_start_watering(uint8_t  zone)
{
    IrrigationRequest request{
        RequestType::StartWatering,
        zone
    };

    return xQueueSend(requestQueue_, &request, 0) == pdPASS;
}

bool IrrigationController::request_stop_watering(uint8_t  zone)
{
    IrrigationRequest request{
        RequestType::StopWatering,
        zone
    };

    return xQueueSend(requestQueue_, &request, 0) == pdPASS;
}

std::optional<IrrigationRequest> IrrigationController::get_next_request()
{
    IrrigationRequest request;

    if (xQueueReceive(requestQueue_, &request, 0) != pdPASS)
        return std::nullopt;

    return request;
}

void IrrigationController::set_event_listener(IrrigationEventListener *listener)
{
    eventListener_ = listener;
}

FlowMeter &IrrigationController::get_flow_meter()
{
    return flowMeter_;
}

WaterUsage &IrrigationController::get_water_usage()
{
    return waterUsage_;
}

uint64_t IrrigationController::get_water_used_delta()
{
    return waterUsedDelta_ml_;
}

void IrrigationController::configure_valves()
{
    valves_[0] = Valve(
        HardwareConfig::VALVE_1_CONTROL_PIN_1, 
        HardwareConfig::VALVE_1_CONTROL_PIN_2, 
        HardwareConfig::VALVE_1_DRIVER_ENABLE_PIN
    );
    valves_[1] = Valve(
        HardwareConfig::VALVE_2_CONTROL_PIN_1, 
        HardwareConfig::VALVE_2_CONTROL_PIN_2, 
        HardwareConfig::VALVE_2_DRIVER_ENABLE_PIN
    );
    valves_[2] = Valve(
        HardwareConfig::VALVE_3_CONTROL_PIN_1, 
        HardwareConfig::VALVE_3_CONTROL_PIN_2, 
        HardwareConfig::VALVE_3_DRIVER_ENABLE_PIN
    );
    valves_[3] = Valve(
        HardwareConfig::VALVE_4_CONTROL_PIN_1, 
        HardwareConfig::VALVE_4_CONTROL_PIN_2, 
        HardwareConfig::VALVE_4_DRIVER_ENABLE_PIN
    );
    valves_[4] = Valve(
        HardwareConfig::VALVE_5_CONTROL_PIN_1, 
        HardwareConfig::VALVE_5_CONTROL_PIN_2, 
        HardwareConfig::VALVE_5_DRIVER_ENABLE_PIN
    );
    valves_[5] = Valve(
        HardwareConfig::VALVE_6_CONTROL_PIN_1, 
        HardwareConfig::VALVE_6_CONTROL_PIN_2, 
        HardwareConfig::VALVE_6_DRIVER_ENABLE_PIN
    );
}

IrrigationController::~IrrigationController()
{
    delete state_;
    delete[] valves_;
}
