#include <stdio.h>
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led.h"
#include "Button.h"
#include "sin.h"
#include "esp_timer.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include <math.h>

static const char *TAG = "APP_MAIN";

void blink_task(void *pvParameters)
{
    while (1) {
        ESP_LOGI(TAG, "LED Ligado");
        led_on();
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_LOGI(TAG, "LED Desligado");
        led_off();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void get_button_state(void *pvParameters)
{
    while (1) {
        int button_state = button_get_state();
        ESP_LOGI(TAG, "Estado do botão: %d", button_state);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void set_sinc(void *pvParameters)
{
    
    func_sinc();

}

void serial_monitor_task(void *pvParameters)
{
    while (1) {

        char buffer[32];

        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {

            if (buffer[0] == 'i' || buffer[0] == 'I') {
                ESP_LOGI("SERIAL", ">>> %s <<<", buffer);
                im_sinc();
            }
            else if (buffer[0] == 'd' || buffer[0] == 'D') {
                ESP_LOGI("SERIAL", ">>> %s <<<", buffer);
                deg_sinc();
            }
            else if (buffer[0] == 'r' || buffer[0] == 'R') {
                ESP_LOGI("SERIAL", ">>> %s <<<", buffer);
                reset_sinc();
            }
            else {

                float v1, v2;

                if (sscanf(buffer, "%f,%f", &v1, &v2) == 2) {
                    reset_sinc();
                    config_hz_ts(v1, v2);
                    ESP_LOGI("SERIAL", "Valor 1: %.2f", v1);
                    ESP_LOGI("SERIAL", "Valor 2: %.2f", v2);

                } else {
                    ESP_LOGI("SERIAL", "Formato invalido: %s", buffer);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


void app_main(void)
{
    led_init();
    button_init();
    config_hz_ts(0.1f, 100.000f); // Configura frequência e tempo de amostragem do filtro

    ESP_LOGI(TAG, "Iniciando sistema... criando task do blink");
    esp_log_level_set("*", ESP_LOG_NONE); // Desativa logs de outros componentes para clareza

    

    xTaskCreate(
        set_sinc,       
        "set_sinc",     
        4096,
        NULL,           
        10,
        NULL            
    );

    xTaskCreate(
        serial_monitor_task, 
        "serial_task", 
        2048, 
        NULL, 
        5, 
        NULL);
    
    xTaskCreate(
        blink_task,     
        "blink_task",   
        1024,
        NULL,           
        1,
        NULL            
    );

    // teste do grafico
    // xTaskCreate(
    //     test,     // função da task
    //     "test",   // nome
    //     4096,           // stack size
    //     NULL,           // parâmetro
    //     5,              // prioridade
    //     NULL            // handle
    // );

    //teste do botão
    // xTaskCreate(
    //     get_button_state,     // função da task
    //     "get_button_state",   // nome
    //     2048,           // stack size
    //     NULL,           // parâmetro
    //     5,              // prioridade
    //     NULL            // handle
    // );
}