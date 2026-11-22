#pragma once
#include <cstdint>
#include "IrrigationController/IrrigationRequest.h"

class IrrigationEventListener
{
    public:
        virtual ~IrrigationEventListener() = default;
        virtual void on_request_rejected(const IrrigationRequest& request) = 0;
        virtual void on_error(uint8_t error_code, uint8_t zone) = 0;
        virtual void on_watering_started(uint8_t zone) = 0;
        virtual void on_watering_completed(uint8_t zone, float liters) = 0;
        virtual void on_measurements_updated() = 0;
};