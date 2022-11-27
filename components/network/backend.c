#include "backend.h"

/* This tag will be prefixed to the log messages */
static const char *TAG = "backend";

esp_err_t send_to_backend(char *data[32])
{
    ESP_LOGI(TAG, "Sending data to backend: %s", data);

    /* Configure HTTP client to use with Kconfig values */
    char local_response_buffer[2048] = {0};
    esp_http_client_config_t config = {
        .host = CONFIG_WEB_SERVER_ADDR,
        .port = CONFIG_WEB_SERVER_PORT,
        .path = CONFIG_WEB_SERVER_PATH,
        .transport_type = HTTP_TRANSPORT_OVER_TCP,
        .user_data = local_response_buffer,
    };
    /* Create client with above config */
    esp_http_client_handle_t client = esp_http_client_init(&config);
    /* The backend server listens to POST requests */
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    /* The message needs to be formatted as JSON */
    esp_http_client_set_header(client, "Content-Type", "application/json");
    /* Form the actual payload here */
    char *post_data;
    asprintf(&post_data, "{\"chip_id\":\"%s\"}", *data);
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    /* Send data to the backend server */
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status: %d, content_length: %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }
    /* Close connection and free resources */
    esp_http_client_cleanup(client);

    return err;
}
