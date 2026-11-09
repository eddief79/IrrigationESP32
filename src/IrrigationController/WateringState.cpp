#include "IrrigationController/WateringState.h"
#include "IrrigationController/IrrigationController.h"
#include "IrrigationController/ErrorState.h"
#include "IrrigationController/ClosingValveState.h"

WateringState::WateringState(int valve_index)
{
    this->valve_index_ = valve_index;
}

void WateringState::on_enter()
{
    delay_ = 0;
}

void WateringState::update()
{
    if (process_request())
        return;

    // Allow a few seconds for the flow to stabilize before throwing an error.
    if (delay_ < FLOW_CHECK_DELAY_SECONDS) 
        ++delay_;
    
    if (delay_ >= FLOW_CHECK_DELAY_SECONDS && controller_->flowMeter_.getFlowRate() > 0.1f) //TODO: Add a threshold to avoid false positives.
        controller_->transition_to(new ErrorState("No water flow detected in Watering state."));
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
                controller_->transition_to(new ErrorState("Start watering requested in the watering state. Only one valve can be open at a time."));
                return true;
            case RequestType::StopWatering:
                if (request.zone == valve_index_)
                    controller_->transition_to(new ClosingValveState(request.zone));
                else
                    controller_->transition_to(new ErrorState("Stop watering was requested for a zone that is not on."));
                return true;
        }
    }
    return false;
}
