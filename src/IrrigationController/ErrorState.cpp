#include "IrrigationController/ErrorState.h"
#include "GeneralConfig.h"
#include "IrrigationController/IrrigationController.h"
#include "IrrigationController/IrrigationEventListener.h"
#include "WaterUsage.h"
#include <esp_log.h>
#include <utility>

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
}
