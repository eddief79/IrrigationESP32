#pragma once
#include "IrrigationController/StateBase.h"
#include <cstdint>
#include <string>

enum class IrrigationErrorCode : uint8_t
{
    WaterFlowWhileIdle = 1,
    StopRequestedWhileIdle = 2,
    NoWaterFlow = 3,
    MaximumDurationExceeded = 4,
    MaximumVolumeExceeded = 5,
    StartRequestedWhileWatering = 6,
    StopRequestedForInactiveZone = 7
};

class ErrorState : public StateBase
{
    private:
        std::string error_message_;
        IrrigationErrorCode error_code_;
        uint8_t active_zone_;

    public:
        ErrorState(std::string error_message, IrrigationErrorCode error_code, uint8_t active_zone = 0xFF);
        void on_enter() override;
        void update() override;
};