/*! ============================================================================
 *
 *  Copyright(c) JV Gomes. All rights reserved.
 *
 *  Tarefa 2:
 *  @brief      Leitor dos sinais analogicos do joystick
 *
 *  @file	    main.c
 *  @author     Joao Vitor G. de Oliveira
 *  @date	    14 Abril 2025
 *  @version    1.0
 * 
============================================================================ */

/* ============================   LIBRARIES  =============================== */
#include <stdio.h>             // Biblioteca padrão para entrada e saída, utilizada para printf.
#include "pico/stdlib.h"       // Biblioteca padrão para funções básicas do Pico, como GPIO e temporização.
#include "hardware/adc.h"      // Biblioteca para controle do ADC (Conversor Analógico-Digital).

/* =============================   MACROS   ================================ */
#define VRX_PIN 26    // Define o pino GP26 para o eixo X do joystick (Canal ADC0).
#define VRY_PIN 27    // Define o pino GP27 para o eixo Y do joystick (Canal ADC1).
#define SW_PIN 22     // Define o pino GP22 para o botão do joystick (entrada digital).

/* ========================   FUNCTION PROTOTYPE   ========================= */

void Joystick_read(uint16_t *vrx, uint16_t *vry, bool *sw); //Realiza a leitura do joystick

/* =========================   GLOBAL VARIABLES   ========================== */


/* ===========================   MAIN FUNCTION   =========================== */
int main() {

    // Inicializa a comunicação serial para permitir o uso de printf.
    stdio_init_all();

    // Inicializa o módulo ADC do Raspberry Pi Pico.
    adc_init();

    // Configura o pino GP26 para leitura analógica do ADC.
    adc_gpio_init(VRX_PIN);

    // Configura o pino GP27 para leitura analógica do ADC.
    adc_gpio_init(VRY_PIN);

    // Configura o pino do botão como entrada digital com pull-up interno.
    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);

    // Loop infinito para ler continuamente os valores do joystick e do botão.
    while (true) 
    {
        uint16_t vrx_value, vry_value;
        bool sw_value;

        // Realiza a leitura dos valores do joystick
        Joystick_read(&vrx_value, &vry_value, &sw_value);

        // Imprime os valores lidos na comunicação serial.
        // VRX e VRY mostram a posição do joystick, enquanto SW mostra o estado do botão.
        printf("VRX: %u, VRY: %u, SW: %d\n", vrx_value, vry_value, sw_value);

        // Introduz um atraso de 250 milissegundos antes de repetir a leitura.
        sleep_ms(250);
    }

    return 0;

}/* end main */

/* =======================  DEVELOPMENT OF FUNCTIONS ======================= */

/*! ---------------------------------------------------------------------------
 *  @brief  Lê os valores do joystick analógico (eixos X e Y) e do botão digital.
 *
 *  @param[out] *vrx Ponteiro para armazenar o valor lido do eixo X (VRX),
 *                   faixa de 0 a 4095 (resolução de 12 bits).
 *  @param[out] *vry Ponteiro para armazenar o valor lido do eixo Y (VRY),
 *                   faixa de 0 a 4095 (resolução de 12 bits).
 *  @param[out] *sw  Ponteiro para armazenar o estado do botão do joystick.
 *                   Recebe 'true' se o botão estiver pressionado, 'false' caso contrário.
 *
 *  @return (void)
 * 
 ----------------------------------------------------------------------------*/
 void Joystick_read(uint16_t *vrx, uint16_t *vry, bool *sw) 
 {
    // Seleciona o canal 0 do ADC (pino GP26) para leitura (VRX).
    adc_select_input(0);
    *vrx = adc_read();   // Lê o valor do eixo X, de 0 a 4095.

    // Seleciona o canal 1 do ADC (pino GP27) para leitura (VRY).
    adc_select_input(1);
    *vry = adc_read();   // Lê o valor do eixo Y, de 0 a 4095.

    // Lê o estado do botão do joystick (SW).
    *sw = gpio_get(SW_PIN) == 0; // 0 indica que o botão está pressionado.
}
/* end program */       