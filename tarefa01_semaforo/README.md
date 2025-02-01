# Tarefa 1: Semáforo com Temporizador Periódico

## Descrição
Esta tarefa tem como objetivo a implementação de um sistema de semáforo utilizando o microcontrolador **Raspberry Pi Pico W**, com temporização periódica para a mudança de sinais. O temporizador é configurado para alterar os LEDs a cada 3 segundos. A mudança de sinal será feita com base em um temporizador repetitivo, utilizando a função `add_repeating_timer_ms()` do Pico SDK.

A solução simula o funcionamento de um semáforo, com LEDs nas cores vermelha, amarela e verde, e utiliza a função de **callback** do temporizador para alternar entre os estados do semáforo.

## Fluxograma Simplificado

- **Início**: O semáforo começa com o LED vermelho aceso.
- **Após 3 segundos**: O LED vermelho desliga, o amarelo acende.
- **Após 3 segundos**: O LED amarelo desliga, o verde acende.
- **Após 3 segundos**: O LED verde desliga, o vermelho acende novamente.

## Requisitos

- **Compilador C**: Necessário para compilar o código.
- **VS Code instalado**: Editor de código necessário para desenvolvimento.
- **Pico SDK configurado**: SDK do Raspberry Pi Pico configurado no ambiente de desenvolvimento.
- **Simulador Wokwi integrado ao VS Code**: Para simulação do projeto sem a necessidade de hardware físico.
- **Git instalado**: Para versionamento de código e clonagem do repositório.
- **Extensão C/C++ instalada no VS Code**: Para suporte ao desenvolvimento em C/C++.
- **Extensão Raspberry Pi Tools instalada no VS Code**: Para facilitar o desenvolvimento para o Raspberry Pi Pico.

## Como Rodar o Projeto

Siga as etapas abaixo para rodar o projeto:

1. **Clone o repositório**:
   No terminal, clone o repositório com o seguinte comando:
   ```bash
   git clone https://github.com/Difarias/semaforo_e_temporizador_embarcatech/tree/master/tarefa01_semaforo
   ```

2. **Abra o projeto no VS Code**:
   Navegue até a pasta do projeto e abra-a no VS Code.

3. **Gere a pasta de build**:
   No terminal integrado do VS Code, execute os seguintes comandos para gerar a pasta de build:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Execute no Wokwi ou no Raspberry Pi Pico**:
   - **Simulação no Wokwi**: Você pode rodar o projeto no Wokwi integrado ao VS Code.
   - **Conecte o Raspberry Pi Pico**: Caso tenha a placa BitDogLab, copie o arquivo `.uf2` gerado para a sua placa.

## Agradecimentos

Gostaria de agradecer à equipe do **Embarcatech** por proporcionar esta atividade desafiadora e enriquecedora, uma vez que me agregou bastante no conhecimento. Durante esse processo, pude desenvolver habilidades práticas em sistemas embarcados e na solução de problemas de forma independente.

---

**Desenvolvedor**  
Diego Farias de Freitas
