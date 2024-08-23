/*********************************************************************************
* File Name: main.c
* Version 1
*
* Description:
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
* "S|XXXX|YYYY|ZZZZZ|AB" - Make a look up table for a cyclic voltammetry experiment. XXXX is the uint16 with the starting number to put in the DAC for the experiment. YYYY is the uint16 with the ending number to put in the dac for the experiment. ZZZZZ is the uint16 to put in the period of the PWM timer to set the sampling rate. A is a char of 'L' or 'C' to make a linear sweep ('L') or a cyclic voltammetry ('C') look up table. B is a char of 'Z' or 'S' to start the waveform at 0 Volts ('Z') or at the value entered in the XXXX field.
* 'R' - Start a cyclic voltammerty experiment with the last look up table that was inputted. To get the data get the ADC Array 0.
* "EX" - Export an ADC array. There are 4 arrays, cyclic voltammetry experiments are stored in the 0 array, the other arrays are used for streaming applications.
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
#include "lut_protocols.h"
#include "usb_protocols.h"
#include "user_selections.h"

/* make buffers for the USB ENDPOINTS */
uint8_t IN_Data_Buffer[MAX_NUM_BYTES];
uint8_t OUT_Data_Buffer[MAX_NUM_BYTES];

char LCD_str[32];  // buffer for LCD screen, make it extra big to avoid overflow
char usb_str[64];  // buffer for string to send to the usb

uint8_t Input_Flag = false;  // if there is an input, set this flag to process it
uint8_t AMux_channel_select = 0;  // Let the user choose to use the two electrode configuration (set to 0) or a three

uint16_t dac_value_hold = 0;


CY_ISR(dacInterrupt) {    // dac interrupt gets pinged

    //  USB_Export_Data((uint8_t*)"Done", 5); // calls a function in an isr but only after the current isr has been disabled

    
    
    
}

CY_ISR(adcInterrupt){
    // ADC_array[0].data[lut_index] = ADC_SigDel_GetResult16(); 
    
    
    
    
    
}

CY_ISR(adcAmpInterrupt){

    LCD_ClearDisplay();    
    current =  ADC_SigDel_GetResult16();
    
    sprintf(LCD_str, "Voltage=%f Current=%f ", voltage, current);
    
    if (echo == ECHO_USB_ON) {           
        USB_Export_Data((uint8_t*)LCD_str, strlen(LCD_str));   
    }  
 }

int main() {
   
    
    /* Default settings*/
    AMux_channel_select = TWO_ELECTRODE_CONFIG;
    
    
    
    
    
    /* Initialize all the hardware and interrupts */
    CyGlobalIntEnable; 
    
    LCD_Start();
    LCD_ClearDisplay();
    LCD_PrintString("Initialisation");
    
    USBUART_Start(0, USBUART_5V_OPERATION);
    LCD_ClearDisplay();
    LCD_PrintString("USB Started");
    
    helper_HardwareSetup(AMux_channel_select);
    LCD_ClearDisplay();
    LCD_PrintString("Potentiostat");
    CyDelay(500);
    
    ADC_SigDel_SelectConfiguration(2, DO_NOT_RESTART_ADC);
    LCD_ClearDisplay();
    LCD_PrintString("ADC Configured");
    
    while(USBUART_GetConfiguration()==0){};  
   
    LCD_ClearDisplay();
    LCD_PrintString("USB Configured");
    
    voltage = 0;
    dac_setvalue(voltage);
    isr_dac_StartEx(dacInterrupt);
    isr_dac_Disable();  // disable interrupt until a voltage signal needs to be given
    
    LCD_ClearDisplay();
    LCD_PrintString("dac Interrupt");
        
    isr_adc_StartEx(adcInterrupt);
    isr_adc_Disable();
    LCD_ClearDisplay();
    LCD_PrintString("adc Interrupt");
        
    CyDelay(500);
    USBUART_CDC_Init();
    LCD_ClearDisplay();
    isr_adcAmp_StartEx(adcAmpInterrupt);
    isr_adcAmp_Disable();   
    LCD_PrintString("USBCDC Interrupt");    
    
    CyDelay(500);
    LCD_PrintString("VDAC AMux");    
    dac_start();
    dac_sleep();
           
       
    
    for(;;) {
       
        if (!Input_Flag) 
        
        {  // make sure any input has already been dealt with
            Input_Flag = USB_CheckInput(OUT_Data_Buffer);  // check if there is a response from the computer
        }
        
        else
        
        {
            LCD_ClearDisplay();
                           
            switch (OUT_Data_Buffer[0]) {   
                
            case CALIBRATE_TIA_ADC: ; // 'B' calibrate the TIA / ADC current measuring circuit
                calibrate_TIA();
                break;
            case SET_PWM_TIMER_COMPARE: ;  // 'C' change the compare value of the PWM to start the adc isr
                PWM_isr_WriteCompare(LUT_Convert2Dec(&OUT_Data_Buffer[2], 5));
                break;
            case SET_PWM_TIMER_PERIOD: ; // 'T' Set the PWM timer period
                user_set_isr_timer(OUT_Data_Buffer);
                break;
            case SET_TIA_ADC: ;  // 'A' Set the TIA resistor, ADC gain and if external resistor is used
                user_setup_TIA_ADC(OUT_Data_Buffer);
                break;

            case RESET_DEVICE: ; // 'X' reset the device by disableing isrs
                user_reset_device();
                break;
            case DEVICE_IDENTIFY: ;  // 'I' identify the device 
                user_identify();
                break;
            case CHANGE_NUMBER_ELECTRODES: ; // 'L' User wants to change the electrode configuration
                
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
                
            case SET_DAC_VALUE: ; // 'D' set the dac value
                dac_setvalue(LUT_Convert2Dec(&OUT_Data_Buffer[2], 4));
                break;
                
            case REPORT_DATA: ; // 'M' run a datapoint toggle
                
                if (echo == ECHO_USB_OFF) 
                {   
                    echo = ECHO_USB_ON;
                }
                else
                {
                    echo = ECHO_USB_OFF;                
                }                        
                break;
            
            case START_HARDWARE: ; // 'H' Start all of the hardware, used to start ASV run
                helper_HardwareWakeup();
                break;
               
                
            case SHORT_TIA: ;  // 's' user wants to short the TIA
                AMux_TIA_input_Connect(2);
                break;
            case STOP_SHORTING_TIA: ;  // 'd' user wants to stop shorting the TIA
                AMux_TIA_input_Disconnect(2);
                break;
            
            }  // end of switch statment
            OUT_Data_Buffer[0] = '0';  // clear data buffer cause it has been processed
            Input_Flag = false;  // turn off input flag because it has been processed
        }
    }  // end of for loop in main
    
}  // end of main

/* [] END OF FILE */
