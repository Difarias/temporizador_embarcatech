#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_AZUL 11 
#define LED_VERMELHO 12 
#define LED_VERDE 13 
#define BOTAO 5

static volatile bool botao_pressionado = false;
static volatile bool acionar_acao = false;

static absolute_time_t ultimo_tempo;

static int contagem_leds = 0;
static int ciclo_ativado = 0;

void configurar_pinos(void);
void alterar_leds(int status);
int64_t alarm_callback(alarm_id_t id, void *dados);
void controlar_led(uint pino, int acao);
void interrupcao_botao(uint gpio, uint32_t eventos);

int main()
{
    stdio_init_all();
    configurar_pinos();

    while (true)
    {
        if (acionar_acao && !botao_pressionado)
        {
            botao_pressionado = true;
            acionar_acao = false;

            if (ciclo_ativado == 0)
            {
                add_alarm_in_ms(1, alarm_callback, NULL, false);
                ciclo_ativado = 1;
            }
        }
        sleep_ms(10);
    }
    return 0;
}

void configurar_pinos(void)
{
    uint pinos_leds[] = {LED_AZUL, LED_VERMELHO, LED_VERDE};
    for (int i = 0; i < 3; i++)
    {
        gpio_init(pinos_leds[i]);
        gpio_set_dir(pinos_leds[i], GPIO_OUT);
        gpio_put(pinos_leds[i], 0);
    }

    gpio_init(BOTAO);
    gpio_set_dir(BOTAO, GPIO_IN);
    gpio_pull_up(BOTAO);
    gpio_set_irq_enabled_with_callback(BOTAO, GPIO_IRQ_EDGE_FALL, true, &interrupcao_botao);
}

void alterar_leds(int status)
{
    switch (status)
    {
    case 0:
        printf("Passaram-se: %d segundos - Todos os LEDs estão ligados.\n", contagem_leds * 3);
        controlar_led(LED_AZUL, 1);
        controlar_led(LED_VERMELHO, 1);
        controlar_led(LED_VERDE, 1);
        break;
    case 1:
        printf("Passaram-se: %d segundos - LED Azul apagado.\n", contagem_leds * 3);
        controlar_led(LED_AZUL, 0);
        break;
    case 2:
        printf("Passaram-se: %d segundos - LED Vermelho apagado.\n", contagem_leds * 3);
        controlar_led(LED_VERMELHO, 0);
        break;
    case 3:
        printf("Passaram-se: %d segundos - LED Verde apagado.\n", contagem_leds * 3);
        controlar_led(LED_VERDE, 0);
        break;
    }
}

int64_t alarm_callback(alarm_id_t id, void *dados)
{
    alterar_leds(contagem_leds);
    contagem_leds++;

    if (contagem_leds >= 4)
    {
        contagem_leds = 0;
        ciclo_ativado = 0;
        printf("Ciclo finalizado! Reiniciando...\n");
    }

    add_alarm_in_ms(3000, alarm_callback, NULL, false);
    return 0;
}

void controlar_led(uint pino, int acao)
{
    gpio_put(pino, acao);
}

void interrupcao_botao(uint gpio, uint32_t eventos)
{
    absolute_time_t tempo_atual = get_absolute_time();
    if (absolute_time_diff_us(ultimo_tempo, tempo_atual) > 200 * 1000){
        if (gpio_get(gpio) == 0) 
            return;

        printf("Botão pressionado! Iniciando ciclo...\n");
        acionar_acao = true;
        ultimo_tempo = tempo_atual;
    }
}
