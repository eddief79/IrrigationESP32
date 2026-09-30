#include "IdleState.h"
#include "OpeningValveState.h"

void IdleState::do_work()
{
    switch (this->controller_->action_requested_)
    {
        case Action::OpenValve:
            this->controller_->transition_to(new OpeningValveState());
            break;        
        case Action::CloseValve:
            this->controller_->transition_to(new OpeningValveState());
            break;
        default:
            break;
    }
}

void IdleState::check_conditions()
{
    
}
