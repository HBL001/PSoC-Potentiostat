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
#include <cytypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    

    
    
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

void helper_ClrRx(void);
void helper_ClrTx(void);
void helper_WipeLCD(void);
void helper_WipeLCD_top(void);
void helper_WipeLCD_bot(void);

char* helper_uint2char(uint8_t *data);


#endif

/* [] END OF FILE */
