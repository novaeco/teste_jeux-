#include "sensors.h"
#include "i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <math.h>

#define SHT31_ADDR 0x44
#define TMP117_ADDR 0x48

static const char *TAG = "sensors";
static i2c_master_dev_handle_t sht31_dev;
static i2c_master_dev_handle_t tmp117_dev;

esp_err_t sensors_init(void)
{
    DEV_I2C_Port port = DEV_I2C_Init();
    (void)port; // bus handle kept internally

    esp_err_t ret = DEV_I2C_Set_Slave_Addr(&sht31_dev, SHT31_ADDR);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set SHT31 address: %s", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    ret = DEV_I2C_Set_Slave_Addr(&tmp117_dev, TMP117_ADDR);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set TMP117 address: %s", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    return ESP_OK;
}

float sensors_read_temperature(void)
{
    if (tmp117_dev == NULL || sht31_dev == NULL) {
        ESP_LOGE(TAG, "Sensor handle is NULL");
        return NAN;
    }

    uint16_t raw_tmp = 0;
    if (DEV_I2C_Read_Word(tmp117_dev, 0x00, &raw_tmp) != ESP_OK) {
        return NAN;
    }
    float tmp117 = (int16_t)raw_tmp * 0.0078125f;

    uint8_t cmd[2] = {0x2C, 0x06};
    if (DEV_I2C_Write_Nbyte(sht31_dev, cmd, 2) != ESP_OK) {
        return NAN;
    }
    vTaskDelay(pdMS_TO_TICKS(15));
    uint8_t data[6] = {0};
    if (DEV_I2C_Read_Nbyte(sht31_dev, 0x00, data, 6) != ESP_OK) {
        return NAN;
    }
    uint16_t raw_sht = (data[0] << 8) | data[1];
    float sht31 = -45.0f + 175.0f * ((float)raw_sht / 65535.0f);

    return (tmp117 + sht31) / 2.0f;
}

float sensors_read_humidity(void)
{
    if (sht31_dev == NULL) {
        ESP_LOGE(TAG, "Sensor handle is NULL");
        return NAN;
    }

    uint8_t cmd[2] = {0x2C, 0x06};
    if (DEV_I2C_Write_Nbyte(sht31_dev, cmd, 2) != ESP_OK) {
        return NAN;
    }
    vTaskDelay(pdMS_TO_TICKS(15));
    uint8_t data[6] = {0};
    if (DEV_I2C_Read_Nbyte(sht31_dev, 0x00, data, 6) != ESP_OK) {
        return NAN;
    }
    uint16_t raw_hum = (data[3] << 8) | data[4];
    return 100.0f * ((float)raw_hum / 65535.0f);
}
