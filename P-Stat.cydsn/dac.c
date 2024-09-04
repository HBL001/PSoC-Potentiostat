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
#include <main.h>

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
uint8 dac_Volts2adc(uint8_t data_buffer[])
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
      
    vdac  = (uint8_t)round((voltage_requested / DAC_RESOLUTION) + DAC_OFFSET);
     
    snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, "Requested Voltage=%dmV Working Electrode=%dmV Adc Setting=%d",(int)voltage_requested,(int) voltage, vdac  );
 
    USB_send (TX_Data, USBUART_BUFFER_SIZE ) ;
    
    
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
