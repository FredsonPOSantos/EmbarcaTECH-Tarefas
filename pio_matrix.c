#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pio_matrix.pio.h"


#define MATRIZ 7
#define NUM_PIXELS 25
#define BOTAO_A 5
#define BOTAO_B 6
#define ATRASO_DEBOUNCE 200
#define LED_VERMELHO 13
#define LED_VERDE 11
#define LED_AZUL 12
#define INTERVALO_PISCA_LED 100
#define ATRASO_ATUALIZACAO_MATRIZ 100
#define NUMERO_MINIMO 0
#define NUMERO_MAXIMO 9

// Matriz de números para exibição
const double numbers[10][25] = {
    // Numero 0
    {0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 1.0, 0.0, 0.0, 1.0,
     1.0, 0.0, 1.0, 0.0, 1.0,
     1.0, 0.0, 0.0, 1.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    // Numero 1
    {0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    // Numero 2
    {0.0, 0.0, 1.0, 1.0, 0.0,
     1.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 1.0},

    // Numero 3
    {0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     0.0, 0.0, 1.0, 1.0, 0.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    // Numero 4
    {0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 1.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0},

    // Numero 5
    {1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0},

    // Numero 6
    {1.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0},

    // Numero 7
    {1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0},

    // Numero 8
    {0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    // Numero 9
    {1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0}};

volatile int numero_atual = 0;
volatile uint32_t ultimo_tempo_debounce_a = 0;
volatile uint32_t ultimo_tempo_debounce_b = 0;

void atualizar_matriz(const double *padrao, PIO pio, uint sm)
{
    uint32_t valor;
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (padrao[i] > 0.0) {
            // Formato WS2812B: 0xGGRRBB00
            // Verde 50% = 0x7F (127)
            valor =  0x7F000000;  // G=00, R=7F, B=00
        } else {
            valor = 0x00000000;  // Desligado
        }
        pio_sm_put_blocking(pio, sm, valor);
    }
}
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (gpio == BOTAO_A && (events & GPIO_IRQ_EDGE_FALL)) {
        if (current_time - ultimo_tempo_debounce_a > ATRASO_DEBOUNCE) {
            ultimo_tempo_debounce_a = current_time;
            numero_atual = (numero_atual < NUMERO_MAXIMO) ? numero_atual + 1 : numero_atual;
        }
    }
    else if (gpio == BOTAO_B && (events & GPIO_IRQ_EDGE_FALL)) {
        if (current_time - ultimo_tempo_debounce_b > ATRASO_DEBOUNCE) {
            ultimo_tempo_debounce_b = current_time;
            numero_atual = (numero_atual > NUMERO_MINIMO) ? numero_atual - 1 : numero_atual;
        }
    }
}
void manipulador_interrupcao_gpio(uint gpio, uint32_t eventos) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());

    if (gpio == BOTAO_A && (eventos & GPIO_IRQ_EDGE_FALL)) {
        if (tempo_atual - ultimo_tempo_debounce_a > ATRASO_DEBOUNCE) {
            ultimo_tempo_debounce_a = tempo_atual;
            numero_atual = (numero_atual < NUMERO_MAXIMO) ? numero_atual + 1 : numero_atual;
        }
    }
    else if (gpio == BOTAO_B && (eventos & GPIO_IRQ_EDGE_FALL)) {
        if (tempo_atual - ultimo_tempo_debounce_b > ATRASO_DEBOUNCE) {
            ultimo_tempo_debounce_b = tempo_atual;
            numero_atual = (numero_atual > NUMERO_MINIMO) ? numero_atual - 1 : numero_atual;
        }
    }
}
void inicializar_leds() {
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
}

void inicializar_botoes() {
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &manipulador_interrupcao_gpio);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    gpio_set_irq_enabled(BOTAO_B, GPIO_IRQ_EDGE_FALL, true);
}

int main() {
    stdio_init_all();
    
    inicializar_leds();
    inicializar_botoes();
    
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    
    pio_matrix_program_init(pio, sm, offset, MATRIZ);
    
    uint32_t tempo_ultimo_pisca = 0;
    bool estado_led = false;
    
    while (true) {
        uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
        
        if (tempo_atual - tempo_ultimo_pisca >= INTERVALO_PISCA_LED) {
            tempo_ultimo_pisca = tempo_atual;
            estado_led = !estado_led;
            gpio_put(LED_VERMELHO, estado_led);
        }
        
        atualizar_matriz(numbers[numero_atual], pio, sm);
        sleep_ms(ATRASO_ATUALIZACAO_MATRIZ);
    }
    
    return 0;
}