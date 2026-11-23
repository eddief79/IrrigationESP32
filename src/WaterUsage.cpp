#include "WaterUsage.h"
#include "nvs.h"
#include "cmath"

void WaterUsage::load_totals()
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("irrigation", NVS_READONLY, &handle);

    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        // Namespace doesn't exist yet.
        // All totals remain zero.
        return;
    }

    ESP_ERROR_CHECK(err);
    char key[16];
    snprintf(key, sizeof(key), "total");
    err = nvs_get_u64(handle, key, &totalVolume_ml_);

    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        totalVolume_ml_ = 0;
    }
    else
    {
        ESP_ERROR_CHECK(err);
    }

    for (size_t zone = 0; zone < GeneralConfig::ZONE_COUNT; ++zone)
    {
        snprintf(key, sizeof(key), "zone%zu", zone + 1);
        err = nvs_get_u64(handle, key, &zoneTotals_ml_[zone]);

        if (err == ESP_ERR_NVS_NOT_FOUND)
        {
            zoneTotals_ml_[zone] = 0;
        }
        else
        {
            ESP_ERROR_CHECK(err);
        }
    }

    nvs_close(handle);
}

void WaterUsage::save_totals()
{
    nvs_handle_t handle;
    ESP_ERROR_CHECK(nvs_open("irrigation", NVS_READWRITE, &handle));

    char key[16];
    snprintf(key, sizeof(key), "total");
    ESP_ERROR_CHECK(nvs_set_u64(handle, key, totalVolume_ml_));

    for (size_t zone = 0; zone < GeneralConfig::ZONE_COUNT; ++zone)
    {
        snprintf(key, sizeof(key), "zone%zu", zone + 1);
        ESP_ERROR_CHECK(nvs_set_u64(handle, key, zoneTotals_ml_[zone]));
    }

    ESP_ERROR_CHECK(nvs_commit(handle));
    nvs_close(handle);
}

void WaterUsage::increment_total(uint8_t zone, uint64_t volume_ml)
{
    if (zone < GeneralConfig::ZONE_COUNT)
    {
        zoneTotals_ml_[zone] += volume_ml;
        totalVolume_ml_ += volume_ml;
        currentCycleTotal_ml_ += volume_ml;
    }
}

uint64_t WaterUsage::get_total_volume_ml()
{
    return totalVolume_ml_;
}

uint64_t WaterUsage::get_zone_volume_ml(uint8_t zone)
{
    if (zone < GeneralConfig::ZONE_COUNT)
    {
        return zoneTotals_ml_[zone];
    }
    return 0;
}

float WaterUsage::get_current_cycle_total_l()
{
    return static_cast<float>(currentCycleTotal_ml_) / 1000.0f;
}

void WaterUsage::reset_current_cycle_total()
{
    currentCycleTotal_ml_ = 0;
}
