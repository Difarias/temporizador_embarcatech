#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos para os LEDs e o botão
#define LED_AZUL 11 
#define LED_VERMELHO 12 
#define LED_VERDE 13 
#define BOTAO 5

// Variáveis globais para controlar o estado do botão e a ação a ser realizada
static volatile bool botao_pressionado = false;
static volatile bool acionar_acao = false;

// Variáveis para controle de tempo e estados dos LEDs
static absolute_time_t ultimo_tempo;
static int contagem_leds = 0;
static int ciclo_ativado = 0;

// Declaração das funções utilizadas no código
void configurar_pinos(void);
void alterar_leds(int status);
void controlar_led(uint pino, int acao);

int64_t alarm_callback(alarm_id_t id, void *dados);
int64_t debounce_callback(alarm_id_t id, void *dados);
void interrupcao_botao(uint gpio, uint32_t eventos);

int main()
{
    // Inicializa a comunicação serial
    stdio_init_all();
    // Configura os pinos dos LEDs e do botão
    configurar_pinos();

    while (true)
    {
        // Se uma ação foi acionada e o botão não está pressionado
        if (acionar_acao && !botao_pressionado)
        {
            botao_pressionado = true;
            acionar_acao = false;

            // Inicia o ciclo de ativação dos LEDs
            if (ciclo_ativado == 0)
            {
                add_alarm_in_ms(1, alarm_callback, NULL, false);
                ciclo_ativado = 1;
            }
        }
        // Pequena pausa para evitar uso excessivo da CPU
        sleep_ms(10);
    }
    return 0;
}

void configurar_pinos(void)
{
    // Array com os pinos dos LEDs
    uint pinos_leds[] = {LED_AZUL, LED_VERMELHO, LED_VERDE};
    
    // Configuração dos LEDs como saída e desligados inicialmente
    for (int i = 0; i < 3; i++)
    {
        gpio_init(pinos_leds[i]);
        gpio_set_dir(pinos_leds[i], GPIO_OUT);
        gpio_put(pinos_leds[i], 0);
    }

    // Configuração do botão como entrada com pull-up
    gpio_init(BOTAO);
    gpio_set_dir(BOTAO, GPIO_IN);
    gpio_pull_up(BOTAO);
    
    // Configuração da interrupção no botão para detectar quando ele é pressionado
    gpio_set_irq_enabled_with_callback(BOTAO, GPIO_IRQ_EDGE_FALL, true, &interrupcao_botao);
}

void alterar_leds(int status)
{
    // Alterna o estado dos LEDs conforme o tempo de execução
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

void controlar_led(uint pino, int acao)
{
    // Define o estado do LED (ligado ou desligado)
    gpio_put(pino, acao);
}

int64_t alarm_callback(alarm_id_t id, void *dados)
{
    // Chama a função para alterar os LEDs
    alterar_leds(contagem_leds);
    contagem_leds++;

    // Se todos os LEDs já passaram pelo ciclo, reinicia
    if (contagem_leds >= 4)
    {
        contagem_leds = 0;
        ciclo_ativado = 0;
        printf("Ciclo finalizado! Reiniciando...\n");
    }

    // Configura um novo alarme para continuar alternando os LEDs a cada 3 segundos
    add_alarm_in_ms(3000, alarm_callback, NULL, false);
    return 0;
}

void interrupcao_botao(uint gpio, uint32_t eventos)
{
    static bool debounce = false;

    // Se o debounce não estiver ativado, processa o evento do botão
    if (!debounce)
    {
        debounce = true; // Ativa o debounce
        printf("Botão pressionado\n");
        acionar_acao = true;

        // Aguarda 200ms antes de liberar o botão novamente
        add_alarm_in_ms(200, debounce_callback, &debounce, false);
    }
}

int64_t debounce_callback(alarm_id_t id, void *dados)
{
    // Libera o debounce, permitindo que o botão seja pressionado novamente
    *(bool *)dados = false;
    return 0; // Retorna 0 para indicar que o alarme não deve ser repetido
}