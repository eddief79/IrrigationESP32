#pragma once
#include "StateBase.h"

class ClosingValveState : public StateBase
{
    public:
        void do_work() override;
        void check_conditions() override;
};