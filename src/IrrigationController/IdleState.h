#pragma once
#include "StateBase.h"

class IdleState : public StateBase
{
    public:
        void do_work() override;
        void check_conditions() override;
};