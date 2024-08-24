/*********************************************************************************
* File Name: main.c
* Version 1
*
* Description: Kyle Lite is a simplified version of Naresuan Potentiostat 
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
* 'I' - Identifies the device, will respond with "PSTAT1" through the USB
* "L|X" - Set electrode configuration to 2 or 3 electrodes. X is the number of electrodes, only 2 or 3 works
* "M|XXXX|YYYY" - Run an amperometry experiment. You need to start to read the data the device will start streaming when given this command. XXXX is an uint16 number to set the DAC value to so the electrodes are at the approriate voltage. YYYY is an uint16 of how many data points to collect in each ADC buffer before exporting the data
* "A|U|X|Y|Z|W" - Set up the TIA and ADC. U is the ADC configuration to use where config 1 uses a Vref of +-2.048 V and config 2 uses +-1.024 V.
*       X is the TIA resistor value index, 
*       a string between 0-7 that sets the TIA resistor value {0-20k, 1-30k, 2-40k, 3-80k, 4-120k, 5-250k, 6-500k, 7-1000k}. 
*       Y is the adc buffer gain setting {1, 2, 4, 8}. Z is 'T' or 'F' for if an external resistor is to be used and the AMux_working_electrode should be set according. 
*       W is 0 or 1 for which user resistor should be selected by the AMux_working_electrode.
* 'B' - Calibrate the ADC and TIA signal chain.
* "VXY" - Check or set the voltage source. X is 'R' to read the voltage source or 'S' to set the voltage source. When setting the voltage source Y should be '2' for the 12-bit dithering VDAC, all other numbers will default to the 8-bit VDAC. When reading the voltage source, the device will return the string "VZ" where Z is the voltage source choice selected before.
* "S|XXXXX" - set the period value of the PWM used as a timer that starts the isrs to change the DAC and read the ADC. XXXXX is a uint16 that is put into the PWM that set the timing with a sample rate of 240 kHz / XXXXX
* "C|XXXXX" - set the compare value of the PWM used as a timer that sets when the DAC changes compared to when the ADC measures.
* 'X' - Reset the device. Disable all isrs and put the hardware to sleep.
* "D|XXXX" - Set the voltage control DAC. XXXX is the value to put in the DAC, which ever one is active.
* 'H' - Wake up all the hardware.
* 's' - Short the TIA so the working electrode can sink more current.
* 'd' - Stop shorting the TIA
*********************************************************************************/

#include <project.h>
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

uint8_t IN_Data_Buffer[MAX_NUM_BYTES];  // buffer for incoming USB Commands
uint8_t OUT_Data_Buffer[MAX_NUM_BYTES]; // buffer for outgoing USB Commands
char LCD_str[32];                       // buffer for LCD screen, make it extra big to avoid overflow
char usb_str[64];                       // buffer for string to send to the usb
uint8_t Input_Flag = false;             // is there an incoming command to process
uint8_t AMux_channel_select = 0;        // two electrode configuration=0, three electrode=1

/* Process the dac interrupt */
CY_ISR(dacInterrupt) {    
    
    LCD_ClearDisplay(); 
    sprintf(LCD_str, "Volt=%.3fV Current=%.2eA \n", voltage, current);  // update the LCD Display
    LCD_PrintString(LCD_str);
}

/* Process the adc interrupt */
CY_ISR(adcInterrupt){
    
    LCD_ClearDisplay(); 
    ADC_value =  ADC_SigDel_GetResult16();
    current = adc_adcToAmp(ADC_value);
    sprintf(LCD_str, "Volt=%.3fV Current=%.2eA \n", voltage, current);  // update the LCD Display
    LCD_PrintString(LCD_str); 
    
}

/* Process the adcAMP interrupt */
CY_ISR(adcAmpInterrupt){

    LCD_ClearDisplay(); 
    ADC_value =  ADC_SigDel_GetResult16();
    current = adc_adcToAmp(ADC_value);
    sprintf(LCD_str, "Volt=%.3fV Current=%.2eA \n", voltage, current);  // update the LCD Display
    LCD_PrintString(LCD_str);
   
    if (echo == ECHO_USB_ON) {           
        USB_Export_Data((uint8_t*)LCD_str, strlen(LCD_str));   
    }  
 }

int main() {
   
    /* Default settings*/
    AMux_channel_select = TWO_ELECTRODE_CONFIG;
    voltage = 0;
    current = 0;
    
    /* Initialize all the hardware and interrupts */
    CyGlobalIntEnable;  
    USBUART_Start(0, USBUART_5V_OPERATION);
    helper_HardwareSetup(AMux_channel_select);
    CyDelay(500);
    ADC_SigDel_SelectConfiguration(2, DO_NOT_RESTART_ADC); 
    while(USBUART_GetConfiguration()==0){};  
    
    dac_Setvalue(voltage);
    isr_dac_StartEx(dacInterrupt);
    isr_dac_Disable();  // disable interrupt until a voltage signal needs to be given
    OUT_Data_Buffer[0] = 65; // ASCII value for 'A'      
    OUT_Data_Buffer[0] = 48; // ASCII value for '0'
    user_setup_TIA_ADC(OUT_Data_Buffer);
    isr_adc_StartEx(adcInterrupt);
    isr_adc_Disable();       
    CyDelay(500);
    
    USBUART_CDC_Init();
    isr_adcAmp_StartEx(adcAmpInterrupt);
    isr_adcAmp_Disable();   
    dac_Start();
    dac_Sleep();
    
     LCD_PrintString("Ready. \n");
               
    for(;;) {
       
        if (!Input_Flag) 
        
        {  
            Input_Flag = USB_CheckInput(OUT_Data_Buffer);  // check if there is a request from the computer host
        }
        
        else
        
        {
            LCD_ClearDisplay();
                           
            switch (OUT_Data_Buffer[0]) {           // select case based upon the first 'L'etter
                
            case CALIBRATE_TIA_ADC: ;               // 'B' calibrate the TIA / ADC current measuring circuit
                calibrate_TIA();
                break;
                
            case SET_PWM_TIMER_COMPARE: ;           // 'C' change the compare value of the PWM to start the adc isr
                PWM_isr_WriteCompare(adc_Convert2Dec(&OUT_Data_Buffer[2], 5));
                break;
                
            case SET_PWM_TIMER_PERIOD: ;            // 'T' Set the PWM timer period
                user_set_isr_timer(OUT_Data_Buffer);
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
                if (AMux_channel_select == TWO_ELECTRODE_CONFIG) 
                    {
                        AMux_channel_select = THREE_ELECTRODE_CONFIG;
                    }
                    else
                    {
                        AMux_channel_select = TWO_ELECTRODE_CONFIG;
                    }
                AMux_electrode_Select(AMux_channel_select);
                break;
            
            case SET_DAC_VALUE: ;                   // 'D' set the dac value
                voltage = adc_Convert2Dec(&OUT_Data_Buffer[2], 4);      // convert the string arguement into an int value
                dac_Setvalue(voltage);                                  // set the dac output 
                CyDelay(20);                                            // let the electrode voltage settle         
                break;
                
            case REPORT_DATA: ;                     // 'M' run a datapoint toggle
                if (echo == ECHO_USB_OFF) 
                {   
                    echo = ECHO_USB_ON;
                }
                else
                {
                    echo = ECHO_USB_OFF;                
                }                        
                break;
                
            case START_HARDWARE: ;                      // 'H' Start taking measurements from the electrode
                helper_HardwareWakeup();                // wake all the hardware ready to go
                dac_Setvalue(voltage);                  // Set the electrode voltage 
                CyDelay(20);                            // let the electrode voltage settle
                ADC_SigDel_StartConvert();              // start the converstion process of the delta sigma adc so it will be ready to read when needed
                CyDelay(10);                            // let the adc get ready
                PWM_isr_WriteCounter(100);              // set the pwm timer so that it will trigger adc isr first
                ADC_value = ADC_SigDel_GetResult16();       // get first adc reading
                isr_dac_Enable();                       // enable the interrupts to start the dac
                isr_adc_Enable();                       // and the adc           
                break;
               
            case SHORT_TIA: ;                       // 's' user wants to short the TIA
                AMux_TIA_input_Connect(2);
                break;
                
            case STOP_SHORTING_TIA: ;               // 'd' user wants to stop shorting the TIA
                AMux_TIA_input_Disconnect(2);
                break;
            
            }  // end of switch statment
            OUT_Data_Buffer[0] = '0';  // clear data buffer cause it has been processed
            Input_Flag = false;  // turn off input flag because it has been processed
        }
    }  // end of for loop in main
    
}  // end of main

/* [] END OF FILE */
