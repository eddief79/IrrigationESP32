#pragma once
#include "cstdint"
#include "GeneralConfig.h"

class WaterUsage
{
    private:
        uint64_t totalVolume_ml_ = 0;
        uint64_t zoneTotals_ml_[GeneralConfig::ZONE_COUNT]{};
        uint64_t currentCycleTotal_ml_ = 0;
    public:
        void load_totals();
        void save_totals();
        void increment_total(uint8_t zone, uint64_t volume_ml);
        uint64_t get_total_volume_ml();
        uint64_t get_zone_volume_ml(uint8_t zone); 
        float get_current_cycle_total_l();
        void reset_current_cycle_total();      
};