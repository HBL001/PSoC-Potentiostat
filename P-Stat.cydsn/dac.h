/*********************************************************************************
* File Name: DAC.h
*
* Description: simple handler for the 8-BIT
*
**********************************************************************************
* Copyright Highland Biosciences Ltd.
* Copyright Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#if !defined(DAC_H)
#define DAC_H

#include <project.h>
#include <math.h>
#include <cytypes.h>
#include <helper_functions.h>
#include <globals.h>
#include <usb_protocols.h>
 
    
/***************************************
*        Function Prototypes
***************************************/  
void dac_Init(void);
void dac_Sleep(void);
void dac_Wakeup(void);
void dac_Setvalue(uint8_t value);
uint8_t dac_Volts2adc(uint8_t data_buffer[]);
    
#endif
/* [] END OF FILE */
