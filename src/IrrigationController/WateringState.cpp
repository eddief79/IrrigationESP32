#include "GeneralConfig.h"
#include "IrrigationController/WateringState.h"
#include "IrrigationController/IrrigationController.h"
#include "IrrigationController/ErrorState.h"
#include "IrrigationController/ClosingValveState.h"
#include "FlowMeter.h"
#include "WaterUsage.h"
#include "esp_log.h"

namespace
{
constexpr char TAG[] = "IRRIGATION_WATERING";
}

WateringState::WateringState(int valve_index)
{
    this->valve_index_ = valve_index;
}

void WateringState::on_enter()
{
    delay_ = 0;
    controller_->start_watering_timer();
    controller_->eventListener_->on_watering_started(valve_index_);
}

void WateringState::update()
{
    controller_->get_water_usage().increment_total(valve_index_, controller_->get_water_used_delta());     
    
    if (process_request())
        return;

    if (check_watering_limits())
        return;

    // Allow a few seconds for the flow to stabilize before throwing an error.
    if (delay_ < GeneralConfig::FLOW_CHECK_DELAY_SECONDS) 
        ++delay_;
    
    if (delay_ >= GeneralConfig::FLOW_CHECK_DELAY_SECONDS 
        && controller_->get_flow_meter().getFlowRate() < GeneralConfig::MIN_WATERING_FLOW_RATE_THRESHOLD)
    {
        controller_->transition_to(new ErrorState(
            "No water flow detected in Watering state.", IrrigationErrorCode::NoWaterFlow, valve_index_));
    }
}

bool WateringState::check_watering_limits()
{
    if (controller_->get_watering_elapsed_seconds() > GeneralConfig::MAX_WATERING_DURATION_SECONDS)
    {
        controller_->transition_to(new ErrorState(
            "Watering cycle exceeded the maximum duration.", IrrigationErrorCode::MaximumDurationExceeded, valve_index_));
        return true;
    }

    if (controller_->get_water_usage().get_current_cycle_total_l() > GeneralConfig::MAX_WATERING_VOLUME_LITERS)
    {
        controller_->transition_to(new ErrorState(
            "Watering cycle exceeded the maximum volume.", IrrigationErrorCode::MaximumVolumeExceeded, valve_index_));
        return true;
    }

    return false;
}

bool WateringState::process_request()
{
    auto request_opt = controller_->get_next_request();

    if (request_opt.has_value())
    {
        IrrigationRequest request = request_opt.value();    

        switch (request.type)
        {
            case RequestType::StartWatering:
                ESP_LOGW(TAG, "Ignoring start request for zone %u while zone %u is watering", 
                    request.zone + 1, valve_index_ + 1);
                controller_->eventListener_->on_error(
                    static_cast<uint8_t>(IrrigationErrorCode::StartRequestedWhileWatering), request.zone);
                return false;
            case RequestType::StopWatering:
                if (request.zone == valve_index_)
                {
                    controller_->transition_to(new ClosingValveState(request.zone));
                    return true;
                }
                else
                {
                    ESP_LOGW(TAG, "Ignoring stop request for zone %u while zone %u is watering", 
                        request.zone + 1, valve_index_ + 1);
                    controller_->eventListener_->on_error(
                        static_cast<uint8_t>(IrrigationErrorCode::StopRequestedForInactiveZone), request.zone);
                    return false;
                }
        }
    }
    return false;
}
