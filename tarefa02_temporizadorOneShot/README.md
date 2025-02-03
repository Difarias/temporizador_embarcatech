# Temporizador de um Disparo (One Shot) - Projeto com Raspberry Pi Pico W

## Objetivo

O objetivo deste projeto é criar um sistema de temporização para o acionamento de LEDs baseado no clique de um botão (pushbutton). O sistema utiliza o microcontrolador Raspberry Pi Pico W e a função `add_alarm_in_ms()` do Pico SDK para gerar um temporizador que alterna os estados dos LEDs em intervalos de 3 segundos.

### Funcionamento

- Ao pressionar o botão, todos os LEDs são acionados.
- Após um tempo de 3 segundos, o primeiro LED será desligado.
- Depois de mais 3 segundos, o segundo LED será desligado.
- Após mais 3 segundos, o último LED será desligado.
- O ciclo reinicia após a desativação do último LED.

## Requisitos para Rodar

Antes de começar, verifique se possui os seguintes itens instalados e configurados corretamente:

### 🎯 Ferramentas Essenciais

- **Compilador para C**: Necessário para transformar o código-fonte em um executável compatível com o microcontrolador.
- **Editor de Código**: Recomendamos o VS Code para facilitar a escrita e depuração do código.

### 🔧 Configuração do Ambiente

- **SDK do Raspberry Pi Pico**: Biblioteca essencial para comunicação e controle do microcontrolador. Certifique-se de que está corretamente configurada no sistema.
- **Simulador Wokwi**: Permite testar o código sem precisar do hardware real, ideal para desenvolvimento rápido e depuração.

### 🔗 Ferramentas de Suporte

- **Git**: Fundamental para controle de versão, backup e colaboração no desenvolvimento do projeto.
- **Extensão C/C++ para VS Code**: Habilita realce de sintaxe, sugestões e depuração para código C/C++.
- **Extensão Raspberry Pi Pico Tools**: Facilita a programação e o upload do firmware para o microcontrolador diretamente pelo VS Code.

💡 **Dica**: Certifique-se de testar a configuração antes de começar a desenvolver para evitar problemas inesperados.

## Como Rodar o Projeto

Siga as etapas abaixo para rodar o projeto:

### 1. **Clone o repositório**:
No terminal, clone o repositório com o seguinte comando:
```bash
git clone https://github.com/Difarias/semaforo_e_temporizador_embarcatech
```

### 2. **Abra o projeto no VS Code**:
Navegue até a pasta do projeto (tarefa02_temporizadorOneShot) e abra-a no VS Code.
```bash
cd tarefa02_temporizadorOneShot/
code .
```

### 3. **Gere a pasta de build**:
No terminal integrado do VS Code, execute os seguintes comandos para gerar a pasta de build:

```bash
mkdir build
cd build
cmake ..
make
```

### 4. **Execute no Wokwi ou no Raspberry Pi Pico**:
- **Simulação no Wokwi**: Você pode rodar o projeto no Wokwi integrado ao VS Code para testar e simular o comportamento do código.
- **Conecte o Raspberry Pi Pico**: Caso tenha a placa BitDogLab, copie o arquivo `.uf2` gerado para a sua placa e execute o projeto diretamente no hardware.

## Descrição do Projeto

Este projeto foi desenvolvido para praticar o uso do temporizador do Raspberry Pi Pico W com a função `add_alarm_in_ms()`. O código aciona LEDs em sequência, alterando seus estados a cada 3 segundos após a interação do usuário através do botão. O controle de interrupção do botão foi implementado para garantir que o botão só altere o estado dos LEDs quando o último LED for desligado, evitando múltiplos acionamentos indesejados durante o ciclo.

### Componentes utilizados

- **Microcontrolador Raspberry Pi Pico W**
- **LEDs (azul, vermelho, verde)**
- **Resistores de 330Ω (para proteção dos LEDs)**
- **Botão (Pushbutton)**

## Agradecimentos

Gostaria de agradecer à equipe do **Embarcatech** por proporcionar esta atividade desafiadora e enriquecedora, uma vez que me agregou bastante no conhecimento. Durante esse processo, pude desenvolver habilidades práticas em sistemas embarcados e na solução de problemas de forma independente.
