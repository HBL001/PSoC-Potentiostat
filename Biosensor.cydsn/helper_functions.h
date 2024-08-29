/*******************************************************************************
* File Name: helper_functions.h
*
* Description:
*  Functions prototypes and their variables used by main.
*
**********************************************************************************
* Copyright Highland Biosciences Ltd
* Copyright Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#if !defined(HELPER_FUNCTIONS_H)
#define HELPER_FUNCTIONS_H

#include <project.h>
#include "cytypes.h"
#include "stdio.h"  
#include "globals.h"
#include "dac.h"
    
/***************************************
*        Variables
***************************************/     
    
extern uint8_t selected_voltage_source;
//extern struct RunParams run_params;
    
    
/***************************************
*        Function Prototypes
***************************************/ 

uint8_t helper_Writebyte_EEPROM(uint8_t data, uint16_t address);
uint8_t helper_Readbyte_EEPROM(uint16_t address);

void helper_HardwareInit(void);
void helper_HardwareEnable(void);
void helper_HardwareSleep(void);
void helper_HardwareWakeup(void);

void helper_InitializeISR(void);

void helper_WipeIN(void);
void helper_WipeOUT(void);
void helper_WipeLCD(void);
void helper_WipeLCD_top(void);
void helper_WipeLCD_bot(void);


#endif

/* [] END OF FILE */
