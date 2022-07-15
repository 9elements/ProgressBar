#include "pn7161.h"

static const char *TAG = "pn7161";

#define I2C_MASTER_SCL_IO           CONFIG_I2C_MASTER_SCL      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           CONFIG_I2C_MASTER_SDA      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              CONFIG_I2C_MASTER_NUM                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          CONFIG_I2C_MASTER_FREQ_HZ                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   CONFIG_I2C_MASTER_TX_BUF_DISABLE                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   CONFIG_I2C_MASTER_RX_BUF_DISABLE                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       CONFIG_I2C_MASTER_TIMEOUT_MS

#define PN7161_SENSOR_ADDR                 0x68        /*!< Slave address of the PN7161 sensor */
#define PN7161_WHO_AM_I_REG_ADDR           0x75        /*!< Register addresses of the "who am I" register */

#define PN7161_PWR_MGMT_1_REG_ADDR         0x6B        /*!< Register addresses of the power managment register */
#define PN7161_RESET_BIT                   7

/**
 * @brief Read a sequence of bytes from a PN7161 registers
 */
static esp_err_t pn7161_register_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    return i2c_master_write_read_device(I2C_MASTER_NUM, PN7161_SENSOR_ADDR, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}

/**
 * @brief Write a byte to a PN7161 register
 */
static esp_err_t pn7161_register_write_byte(uint8_t reg_addr, uint8_t data)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};

    ret = i2c_master_write_to_device(I2C_MASTER_NUM, PN7161_SENSOR_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);

    return ret;
}




// void app_main(void)
// {
//     uint8_t data[2];
//     ESP_ERROR_CHECK(i2c_master_init());
//     ESP_LOGI(TAG, "I2C initialized successfully");

//     /* Read the PN7161 WHO_AM_I register, on power up the register should have the value 0x71 */
//     ESP_ERROR_CHECK(PN7161_register_read(PN7161_WHO_AM_I_REG_ADDR, data, 1));
//     ESP_LOGI(TAG, "WHO_AM_I = %X", data[0]);

//     /* Demonstrate writing by reseting the PN7161 */
//     ESP_ERROR_CHECK(PN7161_register_write_byte(PN7161_PWR_MGMT_1_REG_ADDR, 1 << PN7161_RESET_BIT));

//     ESP_ERROR_CHECK(i2c_driver_delete(I2C_MASTER_NUM));
//     ESP_LOGI(TAG, "I2C unitialized successfully");
// }
