#pragma once

#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/message_buffer.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_log.h>

#include <driver/gpio.h>
#include <driver/i2c.h>

#include "pn7160.h"

#define NO_BITS 0

#define I2C_MASTER_SCL_IO      8      /* GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO      4      /* GPIO number used for I2C master data */
#define I2C_MASTER_NUM         0      /* I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ     500000 /* I2C master clock frequency */
#define I2C_MASTER_TIMEOUT_MS  1000   /* I2C master transmission timeout */
#define I2C_MASTER_TX_BUF_SIZE 0      /* I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_SIZE 0      /* I2C master doesn't need buffer */

#define NFC_IRQ_IQ 10
#define NFC_VEN_IQ 3

void vNfcTask(void *pvParamters);
