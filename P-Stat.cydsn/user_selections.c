/*******************************************************************************
* File Name: user_selections.c
*
* Description:
*
**********************************************************************************
* Copyright Kyle Vitautas Lopin, Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include <project.h>
#include <main.h>                   // TX/RX buffers 
#include <user_selections.h>
#include <calibrate.h>              // calibrator list


void user_setup_electrode (uint8_t data_buffer[])
{

    uint8_t electrode_config = data_buffer[2]-'0';    
      
    helper_ClrTx();

    if (electrode_config == THREE_ELECTRODE_CONFIG) 
       {
            AMux_electrode_Select(THREE_ELECTRODE_CONFIG);
            snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, "3 ELECTRODE MODE");                       
       }
       else
       {
            AMux_electrode_Select(TWO_ELECTRODE_CONFIG);
            snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, "2 ELECTRODE MODE");                
       }
                
    USB_send (TX_Data, USBUART_BUFFER_SIZE ) ;    
}


/******************************************************************************
* Function Name: user_setup_TIA_CONFIG
*******************************************************************************
* Summary: Stop all operations by disabling all the isrs and reset the look up table index to 0
* Global variables:
* Parameters:data_buffer[]
* Return: None
*******************************************************************************/

void user_setup_ADC_CONFIG(uint8_t data_buffer[]) 
{    
    ADC_config = data_buffer[2]-'0';
       
    helper_ClrTx();
    
    if (ADC_config >= 1 || ADC_config <= 4) 
        {
            ADC_DelSig_SelectConfiguration(ADC_config, RESTART_ADC);    
            snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, "ADC CONFIGURATION =%d ",ADC_config) ;                             
        }
        else
        {
            snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, "ADC CONFIGURATION ERROR ") ;                        
        }

    USB_send (TX_Data, USBUART_BUFFER_SIZE ) ; 
}

/******************************************************************************
* Function Name: user_setup_TIA_RESISTOR
*******************************************************************************
*
* Summary: how the values work, basically 0 - 20k, 1 - 30k, etc.
*
* Parameters:data_buffer[]
*
* Return: None
*
*******************************************************************************/

void user_setup_TIA_RESISTOR(uint8_t data_buffer[]) 
{    
    TIA_resistor_value_index = data_buffer[2]-'0'; 
    
    helper_ClrTx();
    
     if (TIA_resistor_value_index >= 0 || TIA_resistor_value_index <= 7) 
        {
            TIA_SetResFB(TIA_resistor_value_index);  
            snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, "TIA RESISTOR =%d ",calibrate_TIA_resistor_list[TIA_resistor_value_index]) ;  
        }
        else
        {
          snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, "TIA RESISTOR ERROR ") ;             
        }
        
    USB_send (TX_Data, USBUART_BUFFER_SIZE ) ; 
}

/******************************************************************************
* Function Name: user_setup_ADC_BUFFER_GAIN
*******************************************************************************
* Summary: is the adc buffer gain setting {1, 2, 4, 8}
* Global variables:
* Parameters:data_buffer[]
* Return: None
*******************************************************************************/

void user_setup_ADC_BUFFER_GAIN(uint8_t data_buffer[]) 
{    
    ADC_buffer_index = data_buffer[2]-'0'; 
    
    helper_ClrTx();
    
    if (ADC_buffer_index >= 0 || ADC_buffer_index <= 3) 
    
    {
        ADC_DelSig_SetBufferGain(ADC_buffer_index); 
        snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, "ADC BUFFER GAIN  =%d",  (unsigned int)pow(2, ADC_buffer_index));      
    }
    else
    {
         snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, "ADC BUFFER GAIN ERROR ");        
    }
    
    USB_send (TX_Data, USBUART_BUFFER_SIZE ) ; 
}
       

/******************************************************************************
* Function Name: user_reset_device
*******************************************************************************
* Summary: Stop all operations by disabling all the isrs and reset the look up table index to 0
* Global variables:
* Parameters: None
* Return:  None
********************************************************************************/
void user_reset_device(void) {

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

void user_identify(void) 
{
helper_ClrTx();
snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, ">> Ready ");    
USB_send (TX_Data, USBUART_BUFFER_SIZE ) ;   
}


/* [] END OF FILE */
