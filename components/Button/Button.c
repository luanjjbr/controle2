#include "driver/gpio.h"
#include "button.h"

#define BOOT_BUTTON_PIN GPIO_NUM_4

void button_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BOOT_BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,   // Ativa resistor interno (evita ruído)
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE      // Por enquanto, sem interrupções
    };
    gpio_config(&io_conf);
}

int button_get_state(void)
{
    // Retorna 0 se pressionado (devido ao Pull-up) e 1 se solto
    return gpio_get_level(BOOT_BUTTON_PIN);
}