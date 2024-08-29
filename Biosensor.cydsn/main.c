/*********************************************************************************
* File Name: main.c
* Version 1
*
* Description: Updated version for chronoamperometric testing
*
**********************************************************************************
* Some parts Copyright Highland Biosciences Ltd., Scotland
* Some parts Copyright Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*
* Refactored Aug 8, 2024
*
* PSoC-Potentiostat Commands
* The following are the inputs commands the device will take, all inputs are inputted as ASCII strings.
* 'I' - Identifies the device, will respond through the USB
* "L|X" - Set electrode configuration to 2 or 3 electrodes. X=0 is 2 electrodes, 1=3 electrodes
* "M|XXXX|YYYY" - Run an amperometry experiment. You need to start to read the data the device will start streaming when given this command. XXXX is an uint16 number to set the DAC value to so the electrodes are at the approriate voltage. YYYY is an uint16 of how many data points to collect in each ADC buffer before exporting the data
* "A|U|X|Y" - Set up the TIA and ADC. 
*       U is the ADC configuration to use where config 1 uses a Vref of +-2.048 V and config 2 uses +-1.024 V.
*       X is the TIA resistor value index, a string between 0-7 that sets the TIA resistor value {0-20k, 1-30k, 2-40k, 3-80k, 4-120k, 5-250k, 6-500k, 7-1000k}. 
*       Y is the adc buffer gain setting {1, 2, 4, 8}. Z is 'T' or 'F' for if an external resistor is to be used and the AMux_working_electrode should be set according. 
* 'B' - Calibrate the ADC and TIA signal chain.
* "T|XXXXX" - set the period value of the PWM used as a timer that starts the isrs to change the DAC and read the ADC. XXXXX is a uint16 that is put into the PWM that set the timing with a sample rate of 240 kHz / XXXXX
* "C|XXXXX" - set the compare value of the PWM used as a timer that sets when the DAC changes compared to when the ADC measures.
* 'X' - Reset the device. Disable all isrs and put the hardware to sleep.
* "D|XXX" - Set the voltage control 8 bit DAC. XXX is the value to put in the DAC
* 'H' - Wake up all the hardware, enable All
* 's' - Short the TIA so the working electrode can sink more current.
* 'd' - Stop shorting the TIA
*********************************************************************************/

#include "project.h"
#include "stdio.h"
#include "stdlib.h"

// local files
#include "calibrate.h"
#include "dac.h"
#include "globals.h"
#include "helper_functions.h"
#include "adc.h"
#include "usb_protocols.h"
#include "user_selections.h"



//CY_ISR(isr_dac_Handler) {                              // process the dac interrupt
 
    
  //  dac_Setvalue(dac_Volts);                           // update the output dac 

//}

/* Process the adc interrupt */
CY_ISR(isr_adc_Handler){
    

 while(!ADC_SigDel_IsEndConversion(ADC_SigDel_WAIT_FOR_RESULT)); // Wait for conversion to complete
    
    
 ADC_value =  ADC_SigDel_GetResult16();
 current = adc_adcToAmp(ADC_value);

  sprintf(IN_Data_Buffer, "Voltage=%u mV Current=%E \n", voltage, current);  // update the LCD Display
   USB_Export_Data(IN_Data_Buffer);

 
}

/* Process the adcAMP interrupt */
CY_ISR(adcAmpInterrupt){

   ADC_value =  ADC_SigDel_GetResult16();
   current = adc_adcToAmp(ADC_value);

   helper_WipeLCD();
   sprintf(LCD_str_top, "Voltage %d mV", voltage);  // update the LCD Display

   LCD_ClearDisplay();

   LCD_Position(0,0);
   LCD_PrintString(LCD_str_top);

   LCD_Position(1,0);
   sprintf(LCD_str_bot, "Current %E mA", current);  // update the LCD Display
   LCD_PrintString(LCD_str_bot);

   sprintf(IN_Data_Buffer, "Voltage=%u mV Current=%E \n", voltage, current);  // update the LCD Display
   USB_Export_Data(IN_Data_Buffer);
 }

int main() {

    // Enable global interrupts
    CyGlobalIntEnable;   
     
   
    
    /* Default settings*/
    voltage = 0;                                                // real mV output
    dac_Volts = 128;                                            // dac value ouput
    uint8_t Input_Flag = false;                                 // is there an incoming command to process
 
    /* initialise the system */
    helper_HardwareInit();
    helper_HardwareEnable();
    ADC_SigDel_SelectConfiguration(2, DO_NOT_RESTART_ADC);
    
    
    USBUART_Start(0, USBUART_5V_OPERATION);
    while(!USBUART_GetConfiguration());   
        
    USBUART_CDC_Init();
        
    USB_Export_Data("(c) 2024 Siddharta LLC - Potentiostat rev 1.0 ");
    USB_Export_Data("Ready ");
    USB_Export_Data("> ");
    
    helper_HardwareSleep();

    // isr_dac_StartEx(isr_dac_Handler);
       
    // isr_adcAmp_StartEx(adcAmpInterrupt);
   
    isr_adc_StartEx(isr_adc_Handler);
    
    ADC_SigDel_StartConvert(); // Start continuous conversions
    
    
    for(;;) {
        if (!Input_Flag) 
        {  
            helper_WipeOUT();
            Input_Flag = USB_CheckInput(OUT_Data_Buffer);  // check the computer host
        }
        else
        {
            helper_WipeIN();
            sprintf(IN_Data_Buffer, "> %s",OUT_Data_Buffer );  // update the USB Display
            USB_Export_Data(IN_Data_Buffer);
                        
            switch (OUT_Data_Buffer[0]) {           // select case based upon the first 'L'etter
                
            case CALIBRATE_TIA_ADC: ;               // 'B' calibrate the TIA / ADC current measuring circuit
                calibrate_TIA();
                break;
                               
            case SET_TIA_ADC: ;                     // 'A' Set the TIA resistor, ADC gain and if external resistor is used              
                user_setup_TIA_ADC(OUT_Data_Buffer);
                break;

            case RESET_DEVICE: ;                    // 'X' reset the device by disableing isrs
                user_reset_device();
                break;
                
            case DEVICE_IDENTIFY: ;                 // 'I' identify the device 
                user_identify();
                break;
                
            case CHANGE_NUMBER_ELECTRODES: ;        // 'L' User wants to change the electrode configuration         
                user_setup_electrode(OUT_Data_Buffer);
                break;
            
            case SET_DAC_VALUE: ;                   // 'D' set the dac value, based on the voltage in millivolts
                dac_Volts = dac_Volts2adc(OUT_Data_Buffer);
                dac_Setvalue(dac_Volts);                                  // set the dac output 
                break;
                
            case STOP_HARDWARE: ;                     // 'M' stop data acquisition
                helper_HardwareSleep();                // Sleep the hardware
           
                isr_dac_Disable();
                isr_adc_Disable();
                isr_adcAmp_Disable();
                ADC_SigDel_StopConvert();                           
                break;
                
            case START_HARDWARE: ;                      // 'H' Start taking measurements from the electrode                
                // helper_HardwareWakeup();                // wake the hardware
                isr_adc_ClearPending();
                isr_dac_ClearPending();
                isr_adcAmp_ClearPending();
                isr_dac_Enable();
                isr_adc_Enable();
                ADC_SigDel_StartConvert();
                isr_adcAmp_Enable();  
                break;
               
            case SHORT_TIA: ;                           // 's' user wants to short the TIA
                AMux_TIA_input_Connect(2);
                break;
                
            case STOP_SHORTING_TIA: ;                   // 'd' user wants to stop shorting the TIA
                AMux_TIA_input_Disconnect(2);
                break;
            }                                           // end of switch statment
            
           helper_WipeOUT();                              // clear data buffer cause it has been processed                      
           helper_WipeIN();        
           Input_Flag = false;                         // turn off input flag because it has been processed
            
        }
    }                                                   // end of for loop in main
    
}  // end of main

/* [] END OF FILE */

