#pragma once
#include "StateBase.h"

class WateringState : public StateBase
{
    public:
        void do_work() override;
        void check_conditions() override;
};