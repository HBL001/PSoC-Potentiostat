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
#define STOP_HARDWARE                   'M'
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
#define NUMBER_BITS 12
#define VOLTAGE_RANGE 2048
#define MAX_BIT_VALUE (1 << NUMBER_BITS)
#define NUM_BITS_TWOS_COMP              16
#define TWOS_ROLLOVER (1 << (NUM_BITS_TWOS_COMP - 1))
#define TWOS_SUBTRACT (1 << NUM_BITS_TWOS_COMP)
    
/**************************************
*           DAC Constants
**************************************/    
#define DAC_RESOLUTION              16
#define DAC_OFFSET                  128
#define VIRTUAL_GROUND              2048    
    
// Define the AMux channels
#define AMux_TIA_working_electrode_ch 1

/**************************************
*        EEPROM API Constants
**************************************/
   
#define VDAC_ADDRESS                            0   
#define EEPROM_READ_TEMPERATURE_CORRECT        0

    
/**************************************
*        EEPROM USB Constants
**************************************/
#define MAX_NUM_BYTES 80
    
/**************************************
*        EEPROM LCD Constants
**************************************/

#define MAX_LCD_BYTES 16

    
/**************************************
*        Global Variables
**************************************/   

static const int calibrate_TIA_resistor_list[] = {20, 30, 40, 80, 120, 250, 500, 1000};
static const uint8_t crlf_buffer[] = { 0x0D, 0x0A };

char8 LCD_str_top[MAX_LCD_BYTES];                       // buffer for LCD screen, make it extra big to avoid overflow
char8 LCD_str_bot[MAX_LCD_BYTES];                       // buffer for LCD screen, make it extra big to avoid overflow
char8 IN_Data_Buffer[MAX_NUM_BYTES];         // buffer for string to send Into the Host PC
uint8_t OUT_Data_Buffer[MAX_NUM_BYTES];      // buffer to read Output from host PC

uint8_t ADC_buffer_index;
uint8_t TIA_resistor_value_index;    
uint16_t ADC_value;

float32 uA_per_adc_count;
float32 R_analog_route;

int voltage;
uint8_t dac_Volts;
double current;
uint8_t echo;
uint16_t dac_ground_value;  

/* External variable of the device address located in USBFS.h */
extern uint8_t USB_deviceAdress;


// Use the print statements in the tests, but not on the hardware
#ifndef TESTING
    #define printf(fmt, ...) (0)
#endif


#endif    
/* [] END OF FILE */
