#include <stdio.h>
#include <math.h>
#include <stdbool.h> // Necessário para usar 'true' se desejar
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h"
#include "esp_log.h"
#include "button.h"

static const char *TAG = "SIN_COMPONENT";

float u[3] = {0};
float y[3] = {0};
float w0 = 2.0f * M_PI * 60.0f; 
float ts = 0.0001f;       

float num[3];
float den[3];


float condition = 1.0f;
float condition_2 = 0.0f;

float b1 = 0.0f;
float a1 = 0.0f;


// Função para deslocar os dados (Fila/Shift Register)
void deslocar_dados(float array[], int tamanho) {
    // Começamos de trás para frente para não sobrescrever os dados errados
    for (int i = tamanho - 1; i > 0; i--) {
        array[i] = array[i - 1];
    }
}


void func_sinc(void) {
    while (true) {
        // 1. Move os valores antigos para o passado:
        // u[1] vira u[2], u[0] vira u[1]
        deslocar_dados(u, 3);
        deslocar_dados(y, 3);


            if(!button_get_state()) {
                u[0] =condition; // Pressionado
            } else {
                u[0] =condition_2; // Não pressionado
            }
        
        

        // 3. Calcula a saída atual baseada no passado
        // y[k] = b1*u[k-1] - a1*y[k-1] - 1*y[k-2]
        y[0] = num[1] * u[1] - den[1] * y[1] - den[2] * y[2];
        // Salva: tempo | entrada | saída
        printf(">Entrada:%f\n", u[0]);
        printf(">Saida_Y:%f\n", y[0]);
        // printf("Entrada_u:%f Saida_y:%f\n", u[0], y[0]);
        // ESP_LOGI(TAG, "Estado do botão: %f", y[0]);

        // vTaskDelay(pdMS_TO_TICKS((int)(ts * 1000)));
        vTaskDelay(pdMS_TO_TICKS((int)(ts * 1000))); // Atraso fixo para evitar sobrecarga, ajuste conforme necessário
    }
}

void reset_sinc(void) {
    condition = 1.0f; // Não pressionado
    condition_2 = 0.0f;
    for (int i = 0; i < 3; i++) {
        u[i] = 0.0f;
        y[i] = 0.0f;
    }
}

void im_sinc(void)
{
    u[0] =1.0f; // Pressionado
}

void deg_sinc(void)
{
    condition = 1.0f; // Não pressionado
    condition_2 = 1.0f;
    for (int i = 0; i < 3; i++) {
        u[i] = 1.0f;
    }
}

void config_hz_ts(float new_f, float new_ts) {
    w0 = 2.0f * M_PI * new_f;
    ts = 1/(new_ts*new_f);
    // Coeficientes calculados uma única vez (fora do loop seria melhor ainda)
    b1 = sinf(w0 * ts);
    a1 = -2.0f * cosf(w0 * ts);

    num[0] = 0.0f; num[1] = b1; num[2] = 0.0f;
    den[0] = 1.0f; den[1] = a1; den[2] = 1.0f;
}

// f =3           # frequência do sinal (Hz)
// T = f*50
// N = (T // f)*2
// s=1
// ts = 1 / T
// w0 = 2 * np.pi * f

// # ==============================
// # SISTEMA DISCRETO
// # ==============================
// b1 = np.sin(w0 * ts)
// a1 = -2 * np.cos(w0 * ts)





// void test(void *pvParameters)
// {
//     printf("header temperature:'#e74c3c' humidity:'#3498db' light:'#f39c12'\n");

//     while (1) {
//         int64_t t = esp_timer_get_time() / 1000; // millis equivalente

//         float temp = 20.0f + 5.0f * sinf(t * 0.001f);
//         float humidity = 50.0f + 10.0f * cosf(t * 0.001f);
//         int light = 500 + (int)(200 * sinf(t * 0.002f));

//         printf("%.2f\t%.2f\t%d\n", temp, humidity, light);

//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }

