/*********************************************************************************
* File Name: adc.c
*
* Description: conversions between the adc and real values
*
**********************************************************************************
* Copyright Highland Biosciences Ltd.
* Copyright Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include "adc.h"
#include "globals.h"

uint8_t ADC_buffer_index;
uint8_t ADC_config;

/* convert the ADC value into Amperes using */
//double adc_adcToAmp(uint16_t adcValue)
//{
//    
//    
//    return (adcVolt / calibrate_TIA_resistor_list[TIA_resistor_value_index]);
// }

                                
/* Convert the integer string to a unsigned 16bit */
uint16_t adc_Convert2Dec(const uint8_t array[], const uint8_t len)

{
    uint16_t num = 0;
    for (int i = 0; i < len; i++){
        num = num * 10 + (array[i] - '0');
    }
    return num;
}

/* [] END OF FILE */
