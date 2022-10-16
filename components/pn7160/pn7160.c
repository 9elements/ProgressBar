#include "pn7160.h"

static const char *TAG = "pn7160";

/**
 * @brief Read a sequence of bytes from a PN7160 registers
 */
static esp_err_t pn7160_register_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    return i2c_master_write_read_device(I2C_MASTER_NUM, PN7160_SENSOR_ADDR, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}

/**
 * @brief Write a byte to a PN7160 register
 */
static esp_err_t pn7160_register_write_byte(uint8_t reg_addr, uint8_t data)
{
    uint8_t write_buf[2] = {reg_addr, data};

    return i2c_master_write_to_device(I2C_MASTER_NUM, PN7160_SENSOR_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}

void test_pn7160(void)
{
    uint8_t data[2];
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    /* Read the PN7160 WHO_AM_I register, on power up the register should have the value 0x71 */
    ESP_ERROR_CHECK(pn7160_register_read(PN7160_WHO_AM_I_REG_ADDR, data, 1));
    ESP_LOGI(TAG, "WHO_AM_I = %X", data[0]);

    /* Demonstrate writing by reseting the PN7160 */
    ESP_ERROR_CHECK(pn7160_register_write_byte(PN7160_PWR_MGMT_1_REG_ADDR, 1 << PN7160_RESET_BIT));

    ESP_ERROR_CHECK(i2c_driver_delete(I2C_MASTER_NUM));
    ESP_LOGI(TAG, "I2C unitialized successfully");
}
