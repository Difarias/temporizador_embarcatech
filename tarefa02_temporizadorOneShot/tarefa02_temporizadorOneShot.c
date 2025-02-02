#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define PINO_LED_VERDE 11
#define PINO_LED_AZUL 12
#define PINO_LED_VERMELHO 13
#define PINO_BOTAO 5

#define INTERVALO_LED 3000 // Tempo para cada mudança de LED (em ms)
#define TEMPO_DE_DEBOUNCE 200 // Debounce para o botão (em ms)

static volatile bool estado_botao = false;
static volatile bool acionar_botao = false;
static absolute_time_t ultimo_evento_botao;
static int contador_leds = 0;
static int ciclo_iniciado = 0; // Para controlar se o ciclo foi iniciado

void configurar_pino_led(uint pin);
void configurar_pino_botao(uint pin);
void controle_leds(int estado);
int64_t iniciar_ciclo_leds(alarm_id_t id, void *user_data);
void ativar_led(uint pin);
void desativar_led(uint pin);
void alarme_botao(uint gpio, uint32_t eventos);

int main()
{
    stdio_init_all();

    // Configurar os pinos dos LEDs
    configurar_pino_led(PINO_LED_VERDE);
    configurar_pino_led(PINO_LED_AZUL);
    configurar_pino_led(PINO_LED_VERMELHO);

    // Configurar o pino do botão
    configurar_pino_botao(PINO_BOTAO);

    while (true)
    {
        if (acionar_botao && !estado_botao)
        {
            estado_botao = true; // Bloqueia novas ações enquanto o ciclo está em andamento
            acionar_botao = false;

            // Inicia o ciclo de LEDs
            if (ciclo_iniciado == 0) {
                add_alarm_in_ms(1, iniciar_ciclo_leds, NULL, false); // Inicia o ciclo
                ciclo_iniciado = 1;
            }
        }
        sleep_ms(10); // Pequeno delay para evitar o uso excessivo de CPU
    }

    return 0;
}

void configurar_pino_led(uint pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, 0); // Inicializa os LEDs apagados
}

void configurar_pino_botao(uint pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin); // Resistor de pull-up
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL, true, &alarme_botao);
}

void controle_leds(int estado)
{
    switch (estado)
    {
    case 0:
        printf("Tempo: %d segundos - Todos os LEDs ligados.\n", contador_leds);
        ativar_led(PINO_LED_VERDE);
        ativar_led(PINO_LED_AZUL);
        ativar_led(PINO_LED_VERMELHO);
        break;
    case 1:
        printf("Tempo: %d segundos - LED Azul desligado.\n", contador_leds);
        desativar_led(PINO_LED_VERDE);
        ativar_led(PINO_LED_AZUL);
        ativar_led(PINO_LED_VERMELHO);
        break;
    case 2:
        printf("Tempo: %d segundos - LED Vermelho desligado.\n", contador_leds);
        desativar_led(PINO_LED_AZUL);
        ativar_led(PINO_LED_VERMELHO);
        break;
    case 3:
        printf("Tempo: %d segundos - LED Verde desligado.\n", contador_leds);
        desativar_led(PINO_LED_VERMELHO);
        break;
    default:
        break;
    }
}

int64_t iniciar_ciclo_leds(alarm_id_t id, void *user_data)
{
    controle_leds(contador_leds);

    // Aumenta o contador de LEDs e reinicia o ciclo quando atingir o limite
    contador_leds++;
    if (contador_leds >= 4)
    {
        contador_leds = 0;
        ciclo_iniciado = 0; // Reseta o ciclo para permitir nova execução após reinício
        printf("Ciclo completo! Reiniciando...\n");
    }

    // Reinicia o ciclo após o tempo de atraso
    add_alarm_in_ms(INTERVALO_LED, iniciar_ciclo_leds, NULL, false);
    
    return 0; // Retorno adequado para a assinatura de callback esperada
}

void ativar_led(uint pin)
{
    gpio_put(pin, 1);
}

void desativar_led(uint pin)
{
    gpio_put(pin, 0);
}

void alarme_botao(uint gpio, uint32_t eventos)
{
    absolute_time_t tempo_atual = get_absolute_time();

    // Verifica se o botão foi pressionado, com o debounce
    if (absolute_time_diff_us(ultimo_evento_botao, tempo_atual) > TEMPO_DE_DEBOUNCE * 1000)
    {
        if (estado_botao)
            return;

        printf("Botão pressionado. Iniciando ciclo...\n");
        acionar_botao = true;
        ultimo_evento_botao = tempo_atual;  // Atualiza o último evento
    }
}
