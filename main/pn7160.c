#include "pn7160.h"

/* This tag will be prefixed to the log messages */
static const char *TAG = "pn7160";

esp_err_t reset_pn7160(void) {
    ESP_LOGD(TAG, "called pn7160 reset");

    return ESP_OK;
}

esp_err_t read_data(uint8_t *data, size_t len) {
    ESP_LOGD(TAG, "called read command");

    return ESP_OK;
}

esp_err_t write_data(uint8_t *data, size_t len) {
    ESP_LOGD(TAG, "called write command");

    return ESP_OK;
}
