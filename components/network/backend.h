#pragma once

#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_log.h>

esp_err_t send_to_backend(char *data[32]);
esp_err_t update_screen(void);
