#pragma once

#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO           CONFIG_I2C_MASTER_SCL               /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           CONFIG_I2C_MASTER_SDA               /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              CONFIG_I2C_MASTER_NUM               /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          CONFIG_I2C_MASTER_FREQ_HZ           /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   CONFIG_I2C_MASTER_TX_BUF_DISABLE    /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   CONFIG_I2C_MASTER_RX_BUF_DISABLE    /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       CONFIG_I2C_MASTER_TIMEOUT_MS

#define PN7161_I2C_ADDR             0x50                                /*!< Slave address of the PN7161 chip*/
#define PN7161_I2C_IRQ              16
#define PN7161_I2C_IRQ              17
