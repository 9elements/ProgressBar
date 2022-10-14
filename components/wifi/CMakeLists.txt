idf_component_register(SRCS "wifi.c" INCLUDE_DIRS "." REQUIRES esp_event esp_wifi freertos log lwip nvs_flash wifi_provisioning)
