#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_VERMELHO 11
#define LED_AMARELO  12
#define LED_VERDE    13

void inicializar_leds();
bool repeating_timer_callback(struct repeating_timer *rt);
void imprimir_mensagem(uint32_t tempo);

int estado_semaforo = 0;

int main() {
    stdio_init_all();
    inicializar_leds();

    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    while (true) {
        uint32_t start = to_ms_since_boot(get_absolute_time());
        imprimir_mensagem(start);
        sleep_ms(1000);
    }
}

void inicializar_leds() {
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_put(LED_VERMELHO, 1);

    gpio_init(LED_AMARELO);
    gpio_set_dir(LED_AMARELO, GPIO_OUT);
    gpio_put(LED_AMARELO, 0);

    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERDE, 0);
}

bool repeating_timer_callback(struct repeating_timer *rt) {
    gpio_put(LED_VERMELHO, 0);
    gpio_put(LED_AMARELO, 0);
    gpio_put(LED_VERDE, 0);

    if (estado_semaforo == 0) {
        gpio_put(LED_AMARELO, 1);
        estado_semaforo = 1;
    } else if (estado_semaforo == 1) {
        gpio_put(LED_VERDE, 1);
        estado_semaforo = 2;
    } else {
        gpio_put(LED_VERMELHO, 1);
        estado_semaforo = 0;
    }

    return true;
}

void imprimir_mensagem(uint32_t tempo) {
    static int contador = 0;
    
    switch (contador % 4) {
        case 0:
            printf("[INFO] O tempo desde o boot é de %u ms\n", tempo);
            break;
        case 1:
            printf("[STATUS] Semáforo operando normalmente...\n");
            break;
        case 2:
            printf("[DEBUG] Tempo atual: %u segundos\n", tempo / 1000);
            break;
        case 3:
            printf("[ALERTA] O sistema está funcionando há %u segundos!\n", tempo / 1000);
            break;
    }

    contador++;
}
