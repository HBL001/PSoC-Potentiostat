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
    
#define EXPORT_LUT_LENGTH               'g'
#define EXPORT_LUT                      'l'
#define EXPORT_STREAMING_DATA           'F'
#define EXPORT_ADC_ARRAY                'E'
#define CALIBRATE_TIA_ADC               'B'
#define SET_PWM_TIMER_COMPARE           'C'
#define SET_PWM_TIMER_PERIOD            'T'
#define SET_TIA_ADC                     'A'
#define CHECK_VOLTAGE_SOURCE            'V'
#define START_CYCLIC_VOLTAMMETRY        'R'
#define RESET_DEVICE                    'X'
#define DEVICE_IDENTIFY                 'I'
#define CHANGE_NUMBER_ELECTRODES        'L'
#define CHRONOAMPEROMETRY_HACK          'Q'
#define MAKE_LOOK_UP_TABLE              'S'
#define SET_DAC_VALUE                   'D'
#define START_HARDWARE                  'H'
#define REPORT_DATA                     'M'
#define SHORT_TIA                       's'
#define STOP_SHORTING_TIA               'd'
#define DPV_LUT                         'G'
    
// Electrode definitions
#define TWO_ELECTRODE_CONFIG            0
#define THREE_ELECTRODE_CONFIG          1 

    
// index of start of different parts of input string
#define INDEX_START_VALUE               2
#define INDEX_END_VALUE                 7
#define INDEX_TIMER_VALUE               12
#define INDEX_SWEEP_TYPE                18
#define INDEX_START_VOLT_TYPE           19
// Square wave voltammertry options
#define INDEX_SWV_INC                   12
#define INDEX_SWV_PULSE_HEIGHT          17
#define INDEX_SWV_TIMER_VALUE           23
#define INDEX_SWV_SWEEP_TYPE            28
#define INDEX_SWV_START_VOLT_TYPE       29


/**************************************
*           ADC Constants
**************************************/  
    
// define how big to make the arrays for the lut for dac and how big
// to make the adc data array     
#define MAX_LUT_SIZE 5000
#define ADC_CHANNELS 4
 
    
/**************************************
*           API Constants
**************************************/
#define true                        1
#define false                       0
    
#define VIRTUAL_GROUND              2048  // TODO: make variable

// Define the AMux channels

    
  
#define AMux_V_VDAC_source_ch       0
#define AMux_V_DVDAC_source_ch      1
#define AMux_TIA_working_electrode_ch 1

/**************************************
*        EEPROM API Constants
**************************************/

#define VDAC_NOT_SET 0
#define VDAC_IS_VDAC 1
#define VDAC_IS_DVDAC 2
    
#define VDAC_ADDRESS 0
    
#define EEPROM_READ_TEMPERATURE_CORRECT        0
    
    
/**************************************
*        ECHO TO USB SERIAL
**************************************/
    
#define ECHO_USB_OFF    0
#define ECHO_USB_ON     1
    
    
/**************************************
*        Global Variables
**************************************/   
     
double voltage;
uint16_t dacIn;
double current;
uint8_t echo;
    
uint16_t dac_ground_value;  // value to load in the DAC
    
/* Make global variables needed for the DAC/ADC interrupt service routines */
    
struct TIAMux {  // not used currently
    uint8_t use_extra_resistor;
    uint8_t user_channel;
};
struct TIAMux tia_mux;

// Use the print statements in the tests, but not on the hardware
#ifndef TESTING
    #define printf(fmt, ...) (0)
#endif


#endif    
/* [] END OF FILE */
