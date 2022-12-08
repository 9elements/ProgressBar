#include "wifi.h"

/* This tag will be prefixed to the log messages */
static const char *TAG = "wifi";

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        /* Try connecting to the AP immediatly after the radio is up */
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        /* We connected successfully */
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(TAG, "Connected with IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        /* We got disconnected, retry */
        ESP_LOGI(TAG, "Disconnected. Connecting to the AP again...");
        esp_wifi_connect();
    }
}

static void setup_nvs(void) {
    /* Initialize NVS partition */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        /* NVS partition was truncated and needs to be erased */
        ESP_ERROR_CHECK(nvs_flash_erase());

        /* Retry nvs_flash_init */
        ESP_ERROR_CHECK(nvs_flash_init());
    }
}

void start_wifi(void) {
    /* NVS for phy data and credentials */
    setup_nvs();

    /* Initialize TCP/IP */
    ESP_ERROR_CHECK(esp_netif_init());

    /* Initialize the event loop */
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* Register our event handler for WiFi and IP related events */
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    /* Initialize WiFi including netif with default config */
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    /* Now with the network stack running we can configure the WiFi access */
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG_WIFI_AP_NAME,
            .password = CONFIG_WIFI_AP_PASS,
            .threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_MIN_MODEM));

    /* Lastly start the WiFi radio */
    ESP_ERROR_CHECK(esp_wifi_start());
}
