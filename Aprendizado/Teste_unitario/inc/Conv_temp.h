/*! ============================================================================
 *
 *  Copyright(c) JV Gomes. All rights reserved.
 *
 *  Conversor de temperatura do ADC
 *  @brief      O arquivo descreve as funcoes de controle de temperaatura
 *
 *  @file	    Conv_temp.h
 *  @author     Joao Vitor G. de Oliveira
 *  @date	    28 Abr 2025
 *  @version    1.0
 * 
============================================================================ */
#ifndef CONV_TEMP_H
#define CONV_TEMP_H

#include <stdint.h>

/*! ---------------------------------------------------------------------------
 *  @brief  A Função converte o valor lido do ADC para temperatura em graus
 *          Celsius
 *
 *  @param adc_value    Entrada dos valores de ADC (uint16_t)
 *  
 *  @return temperature Valor de temperatura convertido (float) 
 * 
 ----------------------------------------------------------------------------*/
float adc_to_temperature(uint16_t adc_value);

#endif // CONV_TEMP_H