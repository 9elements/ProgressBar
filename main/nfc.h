#pragma once

#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/message_buffer.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_log.h>

#include <driver/gpio.h>
#include <driver/i2c.h>

#define NO_BITS 0

#define HIGH 1
#define LOW  0

#define I2C_MASTER_SCL_IO      8      /* GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO      4      /* GPIO number used for I2C master data */
#define I2C_MASTER_NUM         0      /* I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ     500000 /* I2C master clock frequency */
#define I2C_MASTER_TIMEOUT_MS  1000   /* I2C master transmission timeout */
#define I2C_MASTER_TX_BUF_SIZE 0      /* I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_SIZE 0      /* I2C master doesn't need buffer */

#define NFC_IRQ_IQ 10
#define NFC_VEN_IQ 3

#define PN7160_I2C_W_ADDR 0x50 /* Slave write address of the PN7160 chip */
#define PN7160_I2C_R_ADDR 0x51 /* Slave read address of the PN7160 chip */

void reset_nfcc(void);
esp_err_t read_data(uint8_t *data, size_t len, TickType_t timeout);
esp_err_t write_data(uint8_t *data, size_t len, TickType_t timeout);
void vNfcTask(void *pvParamters);
