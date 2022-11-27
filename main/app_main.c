#include "app_main.h"

/* This tag will be prefixed to the log messages */
static const char *TAG = "main";

void app_main(void) {
    ESP_LOGI(TAG, "Starting ProgressBar Scanner!");

    /* Connect to the network via WiFi */
    start_wifi();


}
