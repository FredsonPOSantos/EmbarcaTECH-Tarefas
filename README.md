# Projeto: Contador com Display LED Matrix e Interrupções

## 📝 Descrição
Sistema embarcado para Raspberry Pi Pico que implementa um contador utilizando matriz de LEDs WS2812B (NeoPixel) controlado por botões através de interrupções. O projeto demonstra o uso de PIO (Programmable Input/Output) para controle dos LEDs e interrupções para detecção de botões.

## 🛠️ Componentes
- Raspberry Pi Pico
- Matriz 5x5 LEDs WS2812B
- 2 Botões Push
- 3 LEDs (Vermelho, Verde, Azul)
- Resistores 220Ω

## ⚙️ Funcionalidades
- Exibição de números (0-9) em uma matriz 5x5 de LEDs
- Incremento/decremento através de botões
- LED piscante indicando funcionamento
- Debounce por software nos botões
- Controle dos LEDs via PIO
- Tratamento de interrupções para os botões

## 📌 Pinos Utilizados
- GPIO 7: Dados para Matriz LED
- GPIO 5: Botão A (Incremento)
- GPIO 6: Botão B (Decremento)
- GPIO 13: LED Vermelho
- GPIO 11: LED Verde
- GPIO 12: LED Azul

## 🔧 Configuração e Compilação
```bash
mkdir build
cd build
cmake ..
make

...existing code...

## 👨‍💻 Desenvolvedores
- **Fredson Pereira de Oliveira Santos** - Desenvolvimento do contador com matriz LED - [GitHub](https://github.com/FredsonPOSantos)

## 📖 Instruções de Uso
1. **Botões:**
   - Botão A (GPIO 5): Incrementa o número
   - Botão B (GPIO 6): Decrementa o número
   
2. **Indicadores LED:**
   - LED Vermelho: Pisca para indicar funcionamento
   - LED Verde: Status do sistema
   - LED Azul: Indicador de operação

3. **Display Matrix:**
   - Exibe números de 0-9
   - Atualização a cada 100ms
   - Resolução 5x5 pixels

## 🎥 Vídeo Demonstrativo
[https://drive.google.com/file/d/1ncCgV5kaNasDVK81aa9WaB8hnHvjB6p-/view?usp=sharing]

## 📄 Licença
Este projeto está sob a licença MIT.
Copyright (c) 2025 Fredson Pereira de Oliveira Santos

A permissão é concedida, gratuitamente, a qualquer pessoa que obtenha uma cópia
deste software e arquivos de documentação associados (o "Software"), para lidar
com o Software sem restrições, incluindo, sem limitação, os direitos
de usar, copiar, modificar, mesclar, publicar, distribuir, sublicenciar e/ou vender
cópias do Software.