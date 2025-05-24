/*! ============================================================================
 *
 *  Copyright(c) JV Gomes. All rights reserved.
 *
 *  Teste_unitario
 *  @brief      Leitor de temperatrura interna da MCU
 *
 *  @file	    main.c
 *  @author     Joao Vitor G. de Oliveira
 *  @date	    28 Abr 2025
 *  @version    1.0
 * 
============================================================================ */

/* ============================   LIBRARIES  =============================== */
#include <stdio.h>              // Biblioteca padrão para entrada e saída, utilizada para printf.
#include "pico/stdlib.h"        // Biblioteca padrão para funções básicas do Pico, como GPIO e temporização.
#include "hardware/adc.h"       // Biblioteca para controle do ADC (Conversor Analógico-Digital).
#include "Conv_temp.h"          // Biblioteca para conversão de temperatura

/* =============================   MACROS   ================================ */

#define ADC_TEMPERATURE_CHANNEL 4   // Canal ADC que corresponde ao sensor de temperatura interno

/* ========================   FUNCTION PROTOTYPE   ========================= */


/* =========================   GLOBAL VARIABLES   ========================== */


/* ===========================   MAIN FUNCTION   =========================== */
int main(void)  
{
    // Inicializa a comunicação serial para permitir o uso de printf
    stdio_init_all();

    // Inicializa o módulo ADC do Raspberry Pi Pico
    adc_init();

    // Seleciona o canal 4 do ADC (sensor de temperatura interno)
    adc_set_temp_sensor_enabled(true);          // Habilita o sensor de temperatura interno
    adc_select_input(ADC_TEMPERATURE_CHANNEL);  // Seleciona o canal do sensor de temperatura

    // Loop infinito para leitura contínua do valor de temperatura
    while (true) 
    {
        // Lê o valor do ADC no canal selecionado (sensor de temperatura)
        uint16_t adc_value = adc_read();

        // Converte o valor do ADC para temperatura em graus Fahrenheit
        float temperature = adc_to_temperature(adc_value);
        
        //Imprime a temperatura na comunicação serial
        printf("Temperatura: %.2f °C\n", temperature);

        //Atraso de 1000 milissegundos (1 segundo) entre as leituras
        sleep_ms(1000);
    }
    return 0;
}/* end main */

/* =======================  DEVELOPMENT OF FUNCTIONS ======================= */

/* end program */       