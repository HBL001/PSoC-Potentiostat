/*********************************************************************************
* File Name: main.c
* Version 0.4
*
* Description:
*  Main program to use PSoC 5LP as an electrochemcial device
*
**********************************************************************************
 * Copyright Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)


PSoC-Potentiostat Commands
The following are the inputs commands the device will take, all inputs are inputted as ASCII strings.

'I' - Identifies the device, will respond with "USB Test - v04" through the USB

"L|X" - Set electrode configuration to 2 or 3 electrodes. X is the number of electrodes, only 2 or 3 works

"S|XXXX|YYYY|ZZZZZ|AB" - Make a look up table for a cyclic voltammetry experiment. XXXX is the uint16 with the starting number to put in the DAC for the experiment. YYYY is the uint16 with the ending number to put in the dac for the experiment. ZZZZZ is the uint16 to put in the period of the PWM timer to set the sampling rate. A is a char of 'L' or 'C' to make a linear sweep ('L') or a cyclic voltammetry ('C') look up table. B is a char of 'Z' or 'S' to start the waveform at 0 Volts ('Z') or at the value entered in the XXXX field.

'R' - Start a cyclic voltammerty experiment with the last look up table that was inputted. To get the data get the ADC Array 0.

"EX" - Export an ADC array. There are 4 arrays, cyclic voltammetry experiments are stored in the 0 array, the other arrays are used for streaming applications.

"M|XXXX|YYYY" - Run an amperometry experiment. You need to start to read the data the device will start streaming when given this command. XXXX is an uint16 number to set the DAC value to so the electrodes are at the approriate voltage. YYYY is an uint16 of how many data points to collect in each ADC buffer before exporting the data

"FX" - Exprot an ADC array for streamming data where X is the number of the ADC array to get from 0-3.

"A|U|X|Y|Z|W" - Set up the TIA and ADC. U is the ADC configuration to use where config 1 uses a Vref of +-2.048 V and config 2 uses +-1.024 V. X is the TIA resistor value index, a string between 0-7 that sets the TIA resistor value {0-20k, 1-30k, 2-40k, 3-80k, 4-120k, 5-250k, 6-500k, 7-1000k}. Y is the adc buffer gain setting {1, 2, 4, 8}. Z is 'T' or 'F' for if an external resistor is to be used and the AMux_working_electrode should be set according. W is 0 or 1 for which user resistor should be selected by the AMux_working_electrode.

'B' - Calibrate the ADC and TIA signal chain.

"VXY" - Check or set the voltage source. X is 'R' to read the voltage source or 'S' to set the voltage source. When setting the voltage source Y should be '2' for the 12-bit dithering VDAC, all other numbers will default to the 8-bit VDAC. When reading the voltage source, the device will return the string "VZ" where Z is the voltage source choice selected before.

"S|XXXXX" - set the period value of the PWM used as a timer that starts the isrs to change the DAC and read the ADC. XXXXX is a uint16 that is put into the PWM that set the timing with a sample rate of 240 kHz / XXXXX

"C|XXXXX" - set the compare value of the PWM used as a timer that sets when the DAC changes compared to when the ADC measures.

'X' - Reset the device. Disable all isrs and put the hardware to sleep.

"D|XXXX" - Set the voltage control DAC. XXXX is the value to put in the DAC, which ever one is active.

'H' - Wake up all the hardware.

's' - Short the TIA so the working electrode can sink more current.

'd' - Stop shorting the TIA


*********************************************************************************/

#include <project.h>
#include "stdio.h"
#include "stdlib.h"

// local files
#include "calibrate.h"
#include "DAC.h"
#include "globals.h"
#include "helper_functions.h"
#include "lut_protocols.h"
#include "usb_protocols.h"
#include "user_selections.h"


// #define Work_electrode_resistance 1400  // ohms, estimate of resistance from SC block to the working electrode pin

// hack to send small messages
//union small_data_usb_union {
//    uint8 usb[64];
//    int16 data[32];
//};
//union small_data_usb_union amp_union;

/* make buffers for the USB ENDPOINTS */
uint8_t IN_Data_Buffer[MAX_NUM_BYTES];
uint8_t OUT_Data_Buffer[MAX_NUM_BYTES];

char LCD_str[32];  // buffer for LCD screen, make it extra big to avoid overflow
char usb_str[64];  // buffer for string to send to the usb

uint8_t Input_Flag = false;  // if there is an input, set this flag to process it
uint8_t AMux_channel_select = 0;  // Let the user choose to use the two electrode configuration (set to 0) or a three
// electrode configuration (set to 1) by choosing the correct AMux channel

uint8_t adc_recording_channel = 0;
uint16_t lut_length = 3000;  // how long the look up table is,initialize large so when starting isr the ending doesn't get triggered
//uint16_t lut_hold = 0;  // for debugging
uint8_t adc_hold;  // value to hold what adc buffer was just filled
// uint8 counter = 0;  // for debug
uint16_t buffer_size_bytes;  // number of bytes of data stored to export for amperometry experiments
// for amperometry experiments, how many data points to save before exporting the adc buffer
uint16_t buffer_size_data_pts = 4000;  // prevent the isr from firing by initializing to 4000
uint16_t dac_value_hold = 0;


CY_ISR(dacInterrupt)
{
    DAC_SetValue(lut_value);
    lut_index++;
    if (lut_index >= lut_length) { // all the data points have been given
        isr_adc_Disable();
        isr_dac_Disable();
        ADC_array[0].data[lut_index] = 0xC000;  // mark that the data array is done
        helper_HardwareSleep();
        lut_index = 0; 
        USB_Export_Data((uint8_t*)"Done", 5); // calls a function in an isr but only after the current isr has been disabled
    }
    lut_value = waveform_lut[lut_index];
}
CY_ISR(adcInterrupt){
    ADC_array[0].data[lut_index] = ADC_SigDel_GetResult16(); 
}

CY_ISR(adcAmpInterrupt){
    ADC_array[adc_recording_channel].data[lut_index] = ADC_SigDel_GetResult16(); 
    lut_index++;  
    if (lut_index >= buffer_size_data_pts) {
        ADC_array[adc_recording_channel].data[lut_index] = 0xC000; 
        // counter += 1;  // for debug
        lut_index = 0;
        adc_hold = adc_recording_channel;
        adc_recording_channel = (adc_recording_channel + 1) % ADC_CHANNELS;
        
        sprintf(usb_str, "Done%d", adc_hold);  // tell the user the data is ready to pick up and which channel its on
        USB_Export_Data((uint8_t*)usb_str, 6);  // use the 'F' command to retreive the data
    }
}

int main() {
    /* Initialize all the hardware and interrupts */
    CyGlobalIntEnable; 

    LCD_Start();
    LCD_ClearDisplay();
    LCD_PrintString("Initialisation");
    
    USBUART_Start(0, USBUART_5V_OPERATION);
    LCD_ClearDisplay();
    LCD_PrintString("USB Started");
    
    helper_HardwareSetup();
    LCD_ClearDisplay();
    LCD_PrintString("Potentiostat");
    
    ADC_SigDel_SelectConfiguration(2, DO_NOT_RESTART_ADC);
    LCD_ClearDisplay();
    LCD_PrintString("ADC Configured");
    
    while(USBUART_GetConfiguration()==0){};  
    
    LCD_ClearDisplay();
    LCD_PrintString("USB Configured");
    
    isr_dac_StartEx(dacInterrupt);
    isr_dac_Disable();  // disable interrupt until a voltage signal needs to be given
    
    LCD_ClearDisplay();
    LCD_PrintString("dacInterrupt");
    
    
    isr_adc_StartEx(adcInterrupt);
    isr_adc_Disable();
    LCD_ClearDisplay();
    LCD_PrintString("adcInterrupt");
    
        
    USBUART_CDC_Init();
    
    LCD_ClearDisplay();
    LCD_PrintString("USBCDC initialise");
        
    isr_adcAmp_StartEx(adcAmpInterrupt);
    isr_adcAmp_Disable();   
    
    //CyWdtStart(CYWDT_1024_TICKS, CYWDT_LPMODE_NOCHANGE);
    
    //helper_Writebyte_EEPROM(0, VDAC_ADDRESS);
      
    for(;;) {
        //CyWdtClear();
        
     //   USB_Export_Data((uint8*)"Data Exporting", 14);
     //   CyDelay(1000);     
        
        if (Input_Flag == false) {  // make sure any input has already been dealt with
            Input_Flag = USB_CheckInput(OUT_Data_Buffer);  // check if there is a response from the computer
        }
        
        if (Input_Flag == true) {
                LCD_ClearDisplay();
                sprintf(LCD_str, "%.*s", 16, OUT_Data_Buffer);
                LCD_PrintString(LCD_str);
                
            switch (OUT_Data_Buffer[0]) { 
                
            case EXPORT_STREAMING_DATA: ; // 'F' User wants to export streaming data         
                uint8 user_ch1 = OUT_Data_Buffer[1]-'0';
                USB_Export_Data(&ADC_array[user_ch1].usb[0], buffer_size_bytes); 
                break;
                
            case EXPORT_ADC_ARRAY: ; // 'E' User wants to export the data, the user can choose what ADC array to export
                uint8 user_ch = OUT_Data_Buffer[1]-'0';
                if (user_ch <= ADC_CHANNELS) { // check for buffer overflow
                    // 2*(lut_length+2) because the data is 2 times as long as it has to 
                    // be sent as 8-bits and the data is 16 bit, +1 is for the 0xC000 finished signal
                    USB_Export_Data(&ADC_array[user_ch].usb[0], 2*(lut_length+1));  
                    ADC_array[user_ch].data[0] = lut_length;
                    //USB_Export_Data(&ADC_array[user_ch].usb[0], 2*(lut_length+1));  
                }
                else {
                    USB_Export_Data((uint8*)"Error Exporting", 16);
                }
                break;
            case EXPORT_LUT: ; // 'l' export Look up table
                user_export_lut(OUT_Data_Buffer);
                break;
            case EXPORT_LUT_LENGTH: ; // 'g' export lut_length variable
                user_export_lut_length();
                break;
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
            case CHECK_VOLTAGE_SOURCE: ;  // 'V' check if the device should use the dithering VDAC of the VDAC
                user_voltage_source_funcs(OUT_Data_Buffer);
                break;
            case START_CYCLIC_VOLTAMMETRY: ;  // 'R' Start a cyclic voltammetry experiment
                user_start_cv_run();
                break;
            case RESET_DEVICE: ; // 'X' reset the device by disabbleing isrs
                user_reset_device();
                break;
            case DEVICE_IDENTIFY: ;  // 'I' identify the device 
                user_identify();
                break;
            case CHANGE_NUMBER_ELECTRODES: ; // 'L' User wants to change the electrode configuration
                AMux_channel_select = LUT_Convert2Dec(&OUT_Data_Buffer[2], 1) - 2; // user sends 2 or 3 for the # electrode 
                //config, map this to 0 or 1 for the channel the AMux should select
                AMux_electrode_Select(AMux_channel_select);
                break;
            case CHRONOAMPEROMETRY_HACK: ;  // 'Q' Hack to let the device to run a chronoamperometry experiment, not working properly yet
                lut_length = user_chrono_lut_maker(OUT_Data_Buffer);
                break;
            case MAKE_LOOK_UP_TABLE: ; // 'S' make a look up table (lut) for a cyclic voltammetry experiment
                lut_length = user_lookup_table_maker(OUT_Data_Buffer);
                break; 
            case SET_DAC_VALUE: ; // 'D' set the dac value
                DAC_SetValue(LUT_Convert2Dec(&OUT_Data_Buffer[2], 4));
                break;
            case RUN_AMPEROMETRY: ; // 'M' run an amperometric experiment
                adc_recording_channel = 0;
                buffer_size_data_pts = user_run_amperometry(OUT_Data_Buffer);
                buffer_size_bytes = 2*(buffer_size_data_pts + 1); // add 1 bit for the termination code and double size for bytes from uint16 data
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
            case DPV_LUT: ; // G user wants to make a look up table for differential pulse voltammetry
//                LCD_ClearDisplay();
//                LCD_PrintString("Making DPV LUT");
//                lut_length = user_dpv_lut_maker(OUT_Data_Buffer);
                lut_length = user_lookup_table_maker(OUT_Data_Buffer);
                break;
                break;

            }  // end of switch statment
            OUT_Data_Buffer[0] = '0';  // clear data buffer cause it has been processed
            Input_Flag = false;  // turn off input flag because it has been processed
        }
    }  // end of for loop in main
    
}  // end of main

/* [] END OF FILE */
