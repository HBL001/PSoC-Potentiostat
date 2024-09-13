/*******************************************************************************
* File Name: globals.h
*
* Description:
*  Global variables, unions and structures to use in the project
*
**********************************************************************************
 * Copyright Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#if !defined(GLOBALS_H)
#define GLOBALS_H
    
#include "cytypes.h"
#include "stdio.h"  // gets rid of the type errors

/**************************************
*        USB INPUT OPTIONS
**************************************/ 
    
#define CALIBRATE_TIA_ADC               'B'
#define SET_PWM_TIMER_COMPARE           'C'
#define SET_PWM_TIMER_PERIOD            'T'
#define CHECK_VOLTAGE_SOURCE            'V'
#define RESET_DEVICE                    'X'
#define DEVICE_IDENTIFY                 'I'
#define CHANGE_NUMBER_ELECTRODES        'L'
#define SET_DAC_VALUE                   'D'
#define START_HARDWARE                  'H'
#define STOP_HARDWARE                   'M'
#define SET_DAC_VALUE                   'D'

#define SET_ADC_CONFIG                  'E'
#define TIA_RESISTOR                    'F'
#define ADC_BUFFER_GAIN                 'G'
#define SET_VDAC_OFFSET                 'J'

    
// Electrode definitions
#define TWO_ELECTRODE_CONFIG            (0x00u)
#define THREE_ELECTRODE_CONFIG          (0x01u)
    
      
/**************************************
*           API Constants
**************************************/
#define true                            (0x01u)
#define false                           (0x00u)
    
/**************************************
*           ADC Constants
**************************************/     
#define DO_NOT_RESTART_ADC              (0x00u)  
#define RESTART_ADC                     (0x01u)
#define NUM_CONVERSIONS                 (0x04u)
    
/**************************************
*           DAC Constants
**************************************/    
#define DAC_RESOLUTION                  16
#define DAC_OFFSET                      128
#define VIRTUAL_GROUND                  128    
    
/**************************************
*           DELSIG Constants
**************************************/ 
        
/* Constants for IsEndConversion() "retMode" parameter */
#define ADC_DelSig_RETURN_STATUS        (0x01u)
#define ADC_DelSig_WAIT_FOR_RESULT      (0x00u)
    
/**************************************
*           AMux channels
**************************************/   
#define AMux_TIA_working_electrode_ch   (0x01u)
#define VDAC_ADDRESS                    (0x00u)   
#define VDAC_channel                    (0x00u)  

    
/**************************************
*        USB Constants
**************************************/
  
#define USBFS_DEVICE                    (0x00u)  
#define USBUART_BUFFER_SIZE             (64u)
#define LINE_STR_LENGTH                 (20u)
// char8* parity[] = {"None", "Odd", "Even", "Mark", "Space"};
// char8* stop[]   = {"1", "1.5", "2"};

uint8 buffer[USBUART_BUFFER_SIZE];
    
/**************************************
*        EEPROM API Constants
**************************************/
   

#define EEPROM_READ_TEMPERATURE_CORRECT  (0x00u)  
  
/**************************************
*        EEPROM LCD Constants
**************************************/

#define MAX_LCD_BYTES 16

/**************************************
*        Calibration Constants 
**************************************/   

#define AMux_TIA_calibrat_ch             (0x00u)  
#define AMux_TIA_measure_ch              (0x01u)      
#define Number_calibration_points        (0x05u)    


extern int16_t voltage;
extern float current;


/* External variable of the device address located in USBFS.h */
extern uint8_t USB_deviceAdress;

// Use the print statements in the tests, but not on the hardware
#ifndef TESTING
    #define printf(fmt, ...) (0)
#endif


#endif    
/* [] END OF FILE */
