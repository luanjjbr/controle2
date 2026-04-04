#include <stdio.h>
#include "driver/gpio.h"
#include "esp_err.h"
#include "led.h"

#define INTERNAL_LED_PIN GPIO_NUM_2

void led_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << INTERNAL_LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&io_conf));
}

void led_on(void)
{
    gpio_set_level(INTERNAL_LED_PIN, 1);
}

void led_off(void)
{
    gpio_set_level(INTERNAL_LED_PIN, 0);
}