#pragma once

#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_log.h>

esp_err_t send_to_backend(const uint8_t *data);
