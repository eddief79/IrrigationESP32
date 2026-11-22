#pragma once
#include <cstdint>

namespace GeneralConfig
{
    /// @brief The number of zones in the irrigation system.
    constexpr uint8_t ZONE_COUNT = 6;

    /// @brief The number of seconds to wait before checking for water flow in the Idle and watering states.
    /// This allows the flow to stabilize and reduces the chance of raising false errors.
    constexpr uint8_t FLOW_CHECK_DELAY_SECONDS = 3;

    /// @brief The flow rate threshold, in liters per minute, below which the flow is considered to be zero.
    constexpr float ZERO_FLOW_RATE_THRESHOLD = 0.1f;

    /// @brief The flow rate threshold, in liters per minute, below which the flow is considered too low for effective watering.
    constexpr float MIN_WATERING_FLOW_RATE_THRESHOLD = 0.5f;

    /// @brief The number of pulses the flow meter generates per liter of water.
    constexpr float FLOW_METER_PULSES_PER_LITER = 660.0f;
}
