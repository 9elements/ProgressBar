#pragma once

#include <stdio.h>

#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO           6        /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           5        /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0        /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          3400000  /*!< I2C master clock frequency */
#define I2C_MASTER_TIMEOUT_MS       1000     /*!< I2C master transmission timeout */

#define PN7160_I2C_ADDR             0x50     /*!< Slave address of the PN7160/1 chip*/
#define PN7160_I2C_IRQ              16       /*!< IRQ pin of the PN7160/1 chip*/
#define PN7160_SENSOR_ADDR          0x68     /*!< Slave address of the PN7161 sensor */
#define PN7160_WHO_AM_I_REG_ADDR    0x75     /*!< Register addresses of the "who am I" register */
#define PN7160_PWR_MGMT_1_REG_ADDR  0x6B     /*!< Register addresses of the power managment register */
#define PN7160_RESET_BIT            7

void test_pn7160(void);
