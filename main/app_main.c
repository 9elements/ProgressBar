#include "app_main.h"

/* This tag will be prefixed to the log messages */
static const char *TAG = "main";

void app_main(void) {
    ESP_LOGI(TAG, "Starting ProgressBar Scanner!");

    esp_pm_config_esp32c3_t pm_config = {
        .max_freq_mhz = 160,
        .min_freq_mhz = 20,
        .light_sleep_enable = true,
    };
    ESP_ERROR_CHECK(esp_pm_configure(&pm_config));

    /* Connect to the network via WiFi */
    start_wifi();
}
