#include "store.h"
#include "string.h"
#include "freertos/semphr.h"

static const char *TAG = "store";

static state app_state;

static SemaphoreHandle_t config_lock;

esp_err_t initState() {
    config_lock = xSemaphoreCreateMutex();
    if (config_lock == NULL) {
        ESP_LOGE(TAG, "Error creating mutex");
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t takeConfigLock() {
    if (xSemaphoreTake(config_lock, pdMS_TO_TICKS(1000)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed acquiring configuration lock");
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t releaseConfigLock() {
    if (xSemaphoreGive(config_lock) != pdTRUE) {
        ESP_LOGE(TAG, "Failed releasing configuration lock");
        return ESP_FAIL;
    }
    return ESP_OK;
}

state const * getState() {
    return &app_state;
}

esp_err_t setConfigState(config_state * newConfigState) {
    if (takeConfigLock() == ESP_FAIL) {
        return ESP_FAIL;
    }

    if (app_state.config.flight_mode != newConfigState->flight_mode) {
        handleFltModeUpdate(&app_state, newConfigState);
    }

    for (size_t i = 0; i < 3; ++i)
    {
        if (app_state.config.axis[i].inner_d_fc != newConfigState->axis[i].inner_d_fc) {
            handleFcUpdate(&app_state, newConfigState, i);
        }
    }

    memcpy(&app_state.config, newConfigState, sizeof(config_state));


    if (releaseConfigLock() == ESP_FAIL) {
        return ESP_FAIL;
    }

    return ESP_OK;
}