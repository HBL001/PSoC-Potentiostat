/*********************************************************************************
* File Name: adc.h
*
* Description: simple handler for the 12-BIT adc
*
**********************************************************************************
* Copyright Highland Biosciences Ltd.
* Copyright Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/
#if !defined(ADC_H)
#define ADC_H

#include <project.h>
#include "cytypes.h"
#include "globals.h"

uint16_t adc_Convert2Dec(const uint8_t array[], const uint8_t len);

uint8_t adc_VDAC_offset(uint8_t data_buffer[]);
void adc_VDAC_Setvalue(uint8_t value);

extern uint8_t ADC_buffer_index;
extern uint8_t ADC_config;

#endif
/* [] END OF FILE */
