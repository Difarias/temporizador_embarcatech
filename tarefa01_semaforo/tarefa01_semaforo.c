#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos GPIO correspondentes aos LEDs do semáforo
#define LED_VERMELHO 11
#define LED_AMARELO  12
#define LED_VERDE    13

// Declaração das funções utilizadas no código
void inicializar_leds();
bool repeating_timer_callback(struct repeating_timer *timer);
void imprimir_mensagem(uint32_t tempo);

// Variável global para controlar o estado do semáforo
int estado_semaforo = 0;

int main() {
    // Inicializa a comunicação serial para debug
    stdio_init_all();
    
    // Configura os pinos dos LEDs como saída
    inicializar_leds();

    // Configuração do temporizador para alternar o estado do semáforo a cada 3 segundos (3000 ms)
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Controle do tempo para exibição de mensagens no terminal
    uint32_t tempo_iniciado = to_ms_since_boot(get_absolute_time());
    uint32_t tempo_antigo = tempo_iniciado;
    uint32_t intervalo_mensagem = 1000; // Define o intervalo de atualização das mensagens (1 segundo)

    // Loop principal do programa 
    while (true) {
        // Atualiza o tempo atual do sistema
        tempo_iniciado = to_ms_since_boot(get_absolute_time());
        
        // Se passou 1 segundo desde a última mensagem, exibe uma nova mensagem no terminal
        if (tempo_iniciado - tempo_antigo >= intervalo_mensagem) {
            tempo_antigo = tempo_iniciado; // Atualiza o tempo da última exibição
            imprimir_mensagem(tempo_iniciado); // Chama a função que imprime as mensagens
        }

        // Pequeno delay para evitar uso excessivo do processador
        sleep_ms(200);
    }
}

// Função responsável por configurar os LEDs do semáforo
void inicializar_leds() {
    // Configura o LED vermelho como saída e o acende inicialmente
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_put(LED_VERMELHO, 1);

    // Configura o LED amarelo como saída e o mantém apagado
    gpio_init(LED_AMARELO);
    gpio_set_dir(LED_AMARELO, GPIO_OUT);
    gpio_put(LED_AMARELO, 0);

    // Configura o LED verde como saída e o mantém apagado
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERDE, 0);
}

// Função chamada automaticamente a cada 3 segundos para alternar o estado do semáforo
bool repeating_timer_callback(struct repeating_timer *timer) {
    // Desliga todos os LEDs antes de mudar o estado
    gpio_put(LED_VERMELHO, 0);
    gpio_put(LED_AMARELO, 0);
    gpio_put(LED_VERDE, 0);

    // Alterna os estados do semáforo na seguinte ordem: Vermelho -> Amarelo -> Verde
    if (estado_semaforo == 0) {
        gpio_put(LED_AMARELO, 1);  // Liga o LED Amarelo
        estado_semaforo = 1;
    } else if (estado_semaforo == 1) {
        gpio_put(LED_VERDE, 1);    // Liga o LED Verde
        estado_semaforo = 2;
    } else {
        gpio_put(LED_VERMELHO, 1); // Liga o LED Vermelho
        estado_semaforo = 0;
    }

    return true; // Mantém o temporizador ativo para continuar repetindo
}

// Função responsável por imprimir mensagens no terminal a cada 1 segundo
void imprimir_mensagem(uint32_t tempo) {
    static int contador = 0;

    // A cada chamada da função, imprime uma mensagem diferente
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

    contador++; // Atualiza o contador para a próxima mensagem
}
