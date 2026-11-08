#include "IrrigationController/IdleState.h"
#include "IrrigationController/ErrorState.h"
#include "IrrigationController/OpeningValveState.h"
#include "IrrigationController/IrrigationController.h"

void IdleState::on_enter()
{
    delay_ = 0;
}

void IdleState::update()
{
    process_request();

    // Allow a few seconds for the flow to stabilize before throwing an error.
    if (delay_ < FLOW_CHECK_DELAY_SECONDS) 
        ++delay_;

    if (delay_ >= FLOW_CHECK_DELAY_SECONDS && controller_->flowMeter_.getFlowRate() > 0.0f) //TODO: Add a threshold to avoid false positives from noise in the flow meter signal.
    {
        controller_->transition_to(new ErrorState("Water flow detected in Idle state. All valves should be closed."));
    }
}

void IdleState::process_request()
{
    auto request_opt = controller_->get_next_request();

    if (request_opt.has_value())
    {
        IrrigationRequest request = request_opt.value();    

        switch (request.type)
        {
            case RequestType::StartWatering:
                controller_->transition_to(new OpeningValveState(request.zone));
                break;
            case RequestType::StopWatering:
                controller_->transition_to(new ErrorState("Stop watering requested in the idle state. Nothing to stop."));
                break;
        }
    }
}
