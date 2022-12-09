#include "nfc.h"

/* This tag will be prefixed to the log messages */
static const char *TAG = "nfc";

static TaskHandle_t xNfcTaskHandle = NULL;

void IRAM_ATTR scanner_isr_handler(void *arg) { xTaskNotifyFromISR(xNfcTaskHandle, (uint32_t) arg, eNoAction, false); }

static void initialize_scanner(void) {
    ESP_LOGI(TAG, "Setting up GPIOs");

    /* Setting up GPIOs according to config */
    const gpio_config_t irq_pin_conf = {
        .intr_type = GPIO_INTR_POSEDGE,
        .pin_bit_mask = (1ULL << NFC_IRQ_IQ),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&irq_pin_conf));

    const gpio_config_t ven_pin_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = (1ULL << NFC_IRQ_IQ),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&ven_pin_conf));

    /* Installing ISR handler for GPIO interrupt catching */
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_LOWMED));
    ESP_ERROR_CHECK(gpio_isr_handler_add(NFC_IRQ_IQ, scanner_isr_handler, (void *) true));

    i2c_config_t config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
        .clk_flags = I2C_SCLK_SRC_FLAG_LIGHT_SLEEP,
    };

    i2c_param_config(I2C_MASTER_NUM, &config);

    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, config.mode, I2C_MASTER_RX_BUF_SIZE, I2C_MASTER_TX_BUF_SIZE, ESP_INTR_FLAG_INTRDISABLED));
}

void reset_nfcc(void) {
    ESP_LOGD(TAG, "called nfcc reset");

    ESP_ERROR_CHECK(gpio_set_level(NFC_VEN_IQ, HIGH));
    vTaskDelay(100 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(gpio_set_level(NFC_VEN_IQ, LOW));
}

esp_err_t read_data(uint8_t *data, size_t len, TickType_t timeout) {
    ESP_LOGD(TAG, "called read command");

    return i2c_master_write_to_device(I2C_MASTER_NUM, PN7160_I2C_W_ADDR, data, len, timeout);
}

esp_err_t write_data(uint8_t *data, size_t len, TickType_t timeout) {
    ESP_LOGD(TAG, "called write command");

    return i2c_master_read_from_device(I2C_MASTER_NUM, PN7160_I2C_R_ADDR, data, len, timeout);
}

void vNfcTask(void *pvParamters) {
    ESP_LOGI(TAG, "Running NFC task");

    xNfcTaskHandle = xTaskGetCurrentTaskHandle();
    MessageBufferHandle_t xMessageBuffer = (MessageBufferHandle_t *) pvParamters;

    /* Initialize the GPIOs and the I2C subsystem */
    initialize_scanner();

    /* Make sure the IC is in its default state */
    reset_nfcc();

    /* Data to be received from the scanner, zero init length 32 */
    char data[32] = {0};

    /* Infinite loop to listen for notifications */
    while (true) {
        if (xTaskNotifyWait(NO_BITS, ULONG_MAX, NULL, portMAX_DELAY)) {
            ESP_LOGI(TAG, "Got ISR!");
            // TODO: Trigger pn7160
            xMessageBufferSend(xMessageBuffer, data, sizeof(data), portMAX_DELAY);
        }
    }
}
