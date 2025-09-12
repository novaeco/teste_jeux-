#include "gpio.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "gpio_sim";
static uint8_t s_levels[256];

static void gpio_sim_mode(uint16_t Pin, uint16_t Mode)
{
    (void)Pin;
    (void)Mode;
}

static void gpio_sim_int(int32_t Pin, gpio_isr_t isr_handler)
{
    (void)Pin;
    (void)isr_handler;
}

static void gpio_sim_write(uint16_t Pin, uint8_t Value)
{
    s_levels[Pin & 0xFF] = Value;
}

static uint8_t gpio_sim_read(uint16_t Pin)
{
    return s_levels[Pin & 0xFF];
}

static void gpio_sim_feed(void)
{
    ESP_LOGI(TAG, "Simulated feed");
}

static void gpio_sim_water(void)
{
    ESP_LOGI(TAG, "Simulated water");
}

static void gpio_sim_heat(void)
{
    ESP_LOGI(TAG, "Simulated heat");
}

static void gpio_sim_deinit(void)
{
    memset(s_levels, 0, sizeof(s_levels));
}

const actuator_driver_t gpio_sim_driver = {
    .gpio_mode = gpio_sim_mode,
    .gpio_int = gpio_sim_int,
    .digital_write = gpio_sim_write,
    .digital_read = gpio_sim_read,
    .feed = gpio_sim_feed,
    .water = gpio_sim_water,
    .heat = gpio_sim_heat,
    .deinit = gpio_sim_deinit,
};

