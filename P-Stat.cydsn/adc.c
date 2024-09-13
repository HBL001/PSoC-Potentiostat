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
#include "main.h"

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

/******************************************************************************
* Function Name: adc_VDAC_offset
*******************************************************************************
*
* Summary:
*  Set the value of voltage source as a 0-255 value, with 128 as 0V
*  resolution of 16 mV
*
* Parameters:
*  uint8_t vdac: 
*
* Global variables:
* 
*
*******************************************************************************/

uint8_t adc_VDAC_offset(uint8_t data_buffer[])
{  
    uint8_t TiaOffset;
  
    for (int i = 2; i < 7; i++)
        {
            if(data_buffer[i] >= '0' && data_buffer[i]<='9')
            {
                TiaOffset = TiaOffset * 10 + (data_buffer[i] - '0');
            }
        }  
        
    snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, "VDAC OFFSET =%u ", TiaOffset);
 
    USB_send (TX_Data, USBUART_BUFFER_SIZE ) ;
     
    return TiaOffset;
}


void adc_VDAC_Setvalue(uint8_t value)
{
 
    VDAC_TIA_SetValue(value);
        
}

/* [] END OF FILE */
