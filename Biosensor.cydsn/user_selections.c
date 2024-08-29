/*******************************************************************************
* File Name: user_selections.c
*
* Description:
*  Make some of functions the user can call through the USB. 
*  Only for the longer functions to keep main clearer
*
**********************************************************************************

 * Copyright Kyle Vitautas Lopin, Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include <project.h>
#include "adc.h"
#include "user_selections.h"

void user_setup_electrode (uint8_t data_buffer[]) {

uint8_t electrode_config = data_buffer[2]-'0';    

                
    if (electrode_config == THREE_ELECTRODE_CONFIG) 
       {
            AMux_electrode_Select(THREE_ELECTRODE_CONFIG);
            sprintf(IN_Data_Buffer, "3 ELECTRODE MODE");  // update the LCD Display                        
       }
       else
       {
            AMux_electrode_Select(TWO_ELECTRODE_CONFIG);
            sprintf(IN_Data_Buffer, "2 ELECTRODE MODE");  // update the LCD Display      
       }
             
USB_Export_Data(IN_Data_Buffer);  
    
}


/******************************************************************************
* Function Name: user_setup_TIA_ADC
*******************************************************************************
*
* Summary:
*  Set up the TIA and ADC circuit by changing the TIA resistor value, the ADC gain
*    setup if the user wants to use an external resistor
*  Parameters:
*  uint8 data_buffer[]: array of chars with the parameters to set the adc and tia components
*  input is A|U|X|Y|Z|W: where 
*  U is ADC configuration to use where config 1 uses a Vref of +-2.048 V and config 2 uses +-1.024 V
*  X - the TIA resistor value index, a string between 0-7 that sets the TIA resisot value
*  Y - the adc buffer gain setting
* 
*  W - 0 or 1 for which user resistor should be selected by AMux_working_electrode
*  
* Global variables: - found in calibrate.h and used by calibrate.c
*  uint8 TIA_resistor_value_index: index of whick TIA resistor to use, Supplied by USB input
*  uint8 ADC_buffer_index: which ADC buffer is used, gain = 2**ADC_buffer_index
*
* Return:
*  array of 20 bytes is loaded into the USB in endpoint (into the computer)
*
*******************************************************************************/
void user_setup_TIA_ADC(uint8_t data_buffer[]) {
    
    isr_dac_Disable();
    isr_adc_Disable();
    isr_adcAmp_Disable();
    helper_HardwareSleep();
    
    helper_WipeLCD();
    helper_WipeIN();
    
      
    uint8_t adc_config = data_buffer[2]-'0';
    TIA_resistor_value_index = data_buffer[4]-'0';    
    ADC_buffer_index = data_buffer[6]-'0';       
    
    if (adc_config == 1 || adc_config == 2) 
        {
            ADC_SigDel_SelectConfiguration(adc_config, DO_NOT_RESTART_ADC);            
            sprintf(IN_Data_Buffer, "Vref=%u TIA resistor value=fail Adc Buffer Gain=fail ",adc_config);   

            
            if (TIA_resistor_value_index >= 0 || TIA_resistor_value_index <= 7) 
               {
                    TIA_SetResFB(TIA_resistor_value_index);  // see TIA.h for how the values work, basically 0 - 20k, 1 - 30k, etc.
                    sprintf(IN_Data_Buffer, "Vref=%u  TIA resistor value=%u Adc Buffer Gain=fail ",adc_config , calibrate_TIA_resistor_list[TIA_resistor_value_index]);                    
    
                    if (ADC_buffer_index >= 0 || ADC_buffer_index <= 3) 
                        {
                            ADC_SigDel_SetBufferGain(ADC_buffer_index); 
                            sprintf(IN_Data_Buffer, "Vref=%u  TIA resistor value=%u Adc Buffer Gain=%u ",adc_config , calibrate_TIA_resistor_list[TIA_resistor_value_index],  (unsigned int)pow(2, ADC_buffer_index));
                            }
               }
        }    
        else
        {
            sprintf(IN_Data_Buffer, "Vref=fail TIA resistor value=fail Adc Buffer Gain=fail ");   
        }
        
    USB_Export_Data(IN_Data_Buffer);  
    
  
}

/******************************************************************************
* Function Name: user_reset_device
*******************************************************************************
*
* Summary:
*  Stop all operations by disabling all the isrs and reset the look up table index to 0
*  
* Global variables:
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void user_reset_device(void) {
    isr_dac_Disable();
    isr_adc_Disable();
    isr_adcAmp_Disable();
    helper_HardwareSleep();
}

/******************************************************************************
* Function Name: user_identify
*******************************************************************************
*
* Summary:
*  identify the device and test if the usb is working properly, this is what the program sends at the beginning
*  disable all interruptsincase the program has restarted so the device will also reset
*
* Parameters:
*  None
*
* Return:
*  which version of the device is attached to the USB port
*
*******************************************************************************/

void user_identify(void) {
        
    USB_Export_Data("(c) 2024 Siddharta LLC - Potentiostat rev 1.0");
    
    }


/* [] END OF FILE */
