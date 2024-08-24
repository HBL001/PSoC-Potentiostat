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
#include "cytypes.h"
#include "helper_functions.h"
#include "globals.h"
    
/**************************************
*        AMuX API Constants
**************************************/
#define VDAC_channel 0
    
    
/***************************************
*        Variables
***************************************/     
uint8_t selected_voltage_source;
extern uint16_t dac_ground_value;
    
    
/***************************************
*        Function Prototypes
***************************************/  
void dac_Start(void);
void dac_Sleep(void);
void dac_Wakeup(void);
void dac_Setvalue(uint16_t value);
    
#endif
/* [] END OF FILE */
