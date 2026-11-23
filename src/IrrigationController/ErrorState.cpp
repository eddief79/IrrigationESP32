#include "IrrigationController/IdleState.h"
#include "IrrigationController/ErrorState.h"
#include "GeneralConfig.h"
#include "IrrigationController/IrrigationController.h"
#include "IrrigationController/IrrigationEventListener.h"
#include "FlowMeter.h"
#include "WaterUsage.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "utility"

namespace
{
    constexpr char TAG[] = "IRRIGATION_ERROR";
}

ErrorState::ErrorState(std::string error_message, IrrigationErrorCode error_code, uint8_t active_zone)
    : error_message_(std::move(error_message)),
      error_code_(error_code),
      active_zone_(active_zone)
{
}

void ErrorState::on_enter()
{
	ESP_LOGE(TAG, "%s", error_message_.c_str());

	if (active_zone_ < GeneralConfig::ZONE_COUNT)
	{
		controller_->valves_[active_zone_].close();
		controller_->get_water_usage().save_totals();
		const float cycle_volume_l = controller_->get_water_usage().get_current_cycle_total_l();
		controller_->eventListener_->on_watering_completed(active_zone_, cycle_volume_l);
		controller_->get_water_usage().reset_current_cycle_total();
	}
	else if (error_code_ == IrrigationErrorCode::WaterFlowWhileIdle)
	{
		for (uint8_t zone = 0; zone < GeneralConfig::ZONE_COUNT; ++zone)
			controller_->valves_[zone].close();
	}

	controller_->eventListener_->on_error(static_cast<uint8_t>(error_code_), active_zone_);
}

void ErrorState::update()
{
	while (controller_->get_next_request().has_value())
	{
		ESP_LOGW(TAG, "Discarding irrigation request while error is latched");
	}

	update_timer();

	if (restart_conditions_met())
	{
		ESP_LOGI(TAG, "Restart conditions met; returning to idle");
		controller_->transition_to(new IdleState());
	}
}

void ErrorState::update_timer()
{
	if (controller_->get_flow_meter().getFlowRate() > GeneralConfig::ZERO_FLOW_RATE_THRESHOLD)
	{
		flowClearStartTimeUs_ = 0;
		flowClearTimerActive_ = false;
		return;
	}

	nowUs_ = esp_timer_get_time();
	
	if (!flowClearTimerActive_)
	{
		flowClearStartTimeUs_ = nowUs_;
		flowClearTimerActive_ = true;
	}
}

bool ErrorState::no_flow()
{
	if (!flowClearTimerActive_)
		return false;

    const uint64_t clearDurationUs = static_cast<uint64_t>(GeneralConfig::ERROR_RECOVERY_CLEAR_SECONDS) * 1000000ULL;
	return nowUs_ - flowClearStartTimeUs_ >= static_cast<int64_t>(clearDurationUs);
}

bool ErrorState::restart_conditions_met()
{
    return no_flow();
}
