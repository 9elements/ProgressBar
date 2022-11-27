#pragma once

#include <stdio.h>

#include <esp_err.h>
#include <esp_log.h>

#include <driver/i2c.h>

#define PN7160_I2C_W_ADDR 0x50 /* Slave write address of the PN7160 chip */
#define PN7160_I2C_R_ADDR 0x51 /* Slave read address of the PN7160 chip */

esp_err_t reset_pn7160(void);
esp_err_t read_data(uint8_t *data, size_t len);
esp_err_t write_data(uint8_t *data, size_t len);
