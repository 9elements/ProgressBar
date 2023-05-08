#include "app_main.h"

/* This tag will be prefixed to the log messages */
static const char *TAG = "main";

void app_main(void) {
    ESP_LOGI(TAG, "Starting ProgressBar Scanner!");

    esp_pm_config_esp32c3_t pm_config = {
        .max_freq_mhz = 160,
        .min_freq_mhz = 40,
        .light_sleep_enable = true,
    };
    ESP_ERROR_CHECK(esp_pm_configure(&pm_config));

    /* Connect to the network via WiFi */
    start_wifi();

    /* Data to be received from the scanner, zero init length 32 */
    char data[32] = {0};

    /* Now start a services to handle the NFC scanner */
    MessageBufferHandle_t xMessageBuffer = xMessageBufferCreate(sizeof(data));
    xTaskCreate(vNfcTask, "scanner", 2048, &xMessageBuffer, 5, NULL);

    /* Infinite loop to listen for notifications */
    while (true) {
        if (xMessageBufferReceive(xMessageBuffer, (void *) data, sizeof(data), portMAX_DELAY)) {
            send_to_backend(data, sizeof(data));
        }
    }
}
