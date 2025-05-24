/*! ============================================================================
 *
 *  Copyright(c) JV Gomes. All rights reserved.
 *
 *  Conversor de temperatura do ADC
 *  @brief      O arquivo implementa as funcoes de conversao de temperatura
 *
 *  @file	    conv.c
 *  @author     Joao Vitor G. de Oliveira
 *  @date	    28 Abr 2025
 *  @version    1.0
 * 
============================================================================ */
/* ============================   LIBRARIES  =============================== */
#include "Conv_temp.h"

/* =============================   MACROS  ================================= */
// Constantes baseadas na documentação e parâmetros do RP2040
const float ADC_VREF = 3.3f;
const float ADC_RANGE = (1 << 12); // 4096 valores (0 a 4095)
const float ADC_CONVERSION_FACTOR = ADC_VREF / (ADC_RANGE - 1); // Fator para converter valor ADC em Volts (3.3 / 4095.0)

// Constantes da fórmula de temperatura
const float TEMP_SENSOR_VOLTAGE_AT_27C = 0.706f;
const float TEMP_SENSOR_SLOPE = 0.001721f;

/* =======================  DEVELOPMENT OF FUNCTIONS ======================= */

/*! ---------------------------------------------------------------------------
 *  @brief  A Função converte o valor lido do ADC para temperatura em graus
 *          Celsius
 *
 *  @param adc_value    Entrada dos valores de ADC (uint16_t)
 *  
 *  @return temperature Valor de temperatura convertido (float) 
 * 
 ----------------------------------------------------------------------------*/
float adc_to_temperature(uint16_t adc_value) 
{
    if (adc_value > 4095) {
        adc_value = 4095;
    }
    // Constantes fornecidas no datasheet do RP2040
    const float conversion_factor = 3.3f / ((1 << 12)-1);  // Conversão de 12 bits (0-4095) para 0-3.3V
    float voltage = adc_value * conversion_factor;     // Converte o valor ADC para tensão
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;  // Equação fornecida para conversão
    return temperature; // Retorna o valor de temperatura lido

    // float adc_voltage = (float)adc_value * ADC_CONVERSION_FACTOR;
    // // Aplica a fórmula de conversão para Celsius
    // float temperature_c = 27.0f - (adc_voltage - TEMP_SENSOR_VOLTAGE_AT_27C) / TEMP_SENSOR_SLOPE;

    // return temperature_c;
}