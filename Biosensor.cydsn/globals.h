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
#define SET_TIA_ADC                     'A'
#define CHECK_VOLTAGE_SOURCE            'V'
#define RESET_DEVICE                    'X'
#define DEVICE_IDENTIFY                 'I'
#define CHANGE_NUMBER_ELECTRODES        'L'
#define SET_DAC_VALUE                   'D'
#define START_HARDWARE                  'H'
#define REPORT_DATA                     'M'
#define SHORT_TIA                       's'
#define STOP_SHORTING_TIA               'd'
    
// Electrode definitions
#define TWO_ELECTRODE_CONFIG            0
#define THREE_ELECTRODE_CONFIG          1 
    
      
/**************************************
*           API Constants
**************************************/
#define true                        1
#define false                       0
    
/**************************************
*           ADC Constants
**************************************/    
    
#define DO_NOT_RESTART_ADC              0  
#define VIRTUAL_GROUND              2048  // TODO: make variable
#define NUMBER_BITS 12
#define VOLTAGE_RANGE 2048
#define MAX_BIT_VALUE (1 << NUMBER_BITS)
#define NUM_BITS_TWOS_COMP 16
#define TWOS_ROLLOVER (1 << (NUM_BITS_TWOS_COMP - 1))
#define TWOS_SUBTRACT (1 << NUM_BITS_TWOS_COMP)
    
    
// Define the AMux channels
#define AMux_TIA_working_electrode_ch 1

/**************************************
*        EEPROM API Constants
**************************************/
   
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

static const int calibrate_TIA_resistor_list[] = {20, 30, 40, 80, 120, 250, 500, 1000};
uint8_t TIA_resistor_value_index;    
uint16_t ADC_value;

double voltage;
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
