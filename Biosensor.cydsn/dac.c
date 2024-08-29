/*********************************************************************************
* File Name: DAC.c
* Description: simple handler for the 8-BIT
*
**********************************************************************************
* Copyright Highland Biosciences Ltd.
* Copyright Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include <ctype.h> 
#include "dac.h"


/******************************************************************************
* Function Name: dac_volts2adc
*******************************************************************************
*
* Summary: convert the voltage in millivolts to the DAC value, with the ground
* offset 
*
* Parameters: a string value 
*
* Global variables:
*
*******************************************************************************/
uint8_t dac_Volts2adc(uint8_t data_buffer[])
{
    uint8_t vdac = DAC_OFFSET;
    double  voltage_requested = 0;
    
    if (data_buffer[2] == 45)   
    {
        for (int i = 3; i < 8; i++)
        {
            if(data_buffer[i] >= '0' && data_buffer[i]<= '9')
                {
                voltage_requested = voltage_requested * 10 + (data_buffer[i] - '0');
                }
        }  
        
        voltage_requested = -(voltage_requested);
    } 
    else 
    {
        for (int i = 2; i < 7; i++)
        {
            if(data_buffer[i] >= '0' && data_buffer[i]<='9')
            {
                voltage_requested = voltage_requested * 10 + (data_buffer[i] - '0');
            }
        }  
    }
    
    
    double dacEstimate = (voltage_requested / DAC_RESOLUTION) + DAC_OFFSET;
    
    if (data_buffer[2] != 45)   // the value is positive
    {     
        vdac = (uint8_t)ceil(dacEstimate);
    }
    else
    {
        vdac = (uint8_t)floor(dacEstimate);
    }
 
    voltage = (vdac-128)*16;
    
    helper_WipeIN();
    helper_WipeLCD();
                
    sprintf(LCD_str_top, "SET: V %dmV", (int)voltage_requested);  // update the LCD Display   
    sprintf(IN_Data_Buffer, "Requested Voltage=%dmV Working Electrode=%dmV Adc Setting=%d",(int)voltage_requested, voltage, vdac);   
    sprintf(LCD_str_bot, "V=%d ADC=%d", voltage, vdac);
    
    LCD_ClearDisplay();
    USB_Export_Data(IN_Data_Buffer);  
    LCD_Position(0,0);
    LCD_PrintString(LCD_str_top);
    LCD_Position(1,0);
    LCD_PrintString(LCD_str_bot);
    
    return vdac;
}

/******************************************************************************
* Function Name: dac_Setvalue
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
void dac_Setvalue(uint8_t vdac) 
{         
    VDAC_Poise_SetValue(vdac);

}

/* [] END OF FILE */
