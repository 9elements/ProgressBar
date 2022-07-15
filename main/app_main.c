#include "app_main.h"

/* This tag will be prefixed to the log messages */
static const char *TAG = "main";

void app_main(void) {
    ESP_LOGI(TAG, "Started Progress Bar Scanner!");

    int result = rust_main();

    ESP_LOGI(TAG, "Rust returned code: %d", result);

    provision_wifi();
}
