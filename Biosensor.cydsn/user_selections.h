/*******************************************************************************
* File Name: user_selections.h
*
* Description:
*  
*
**********************************************************************************
* Copyright Kyle Vitautas Lopin, Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/
#if !defined(USER_SELECTIONS_H)
#define USER_SELECTIONS_H
    
#include <project.h>
#include "stdio.h"  // gets rid of the type errors
    
#include "globals.h"
#include "helper_functions.h"
#include "usb_protocols.h"
    
/***************************************
*        Function Prototypes
***************************************/  
void user_setup_TIA_ADC(uint8_t data_buffer[]);
void user_identify(void);
void user_set_isr_timer(uint8_t data_buffer[]);
void user_setup_electrode (uint8_t data_buffer[]);
void user_reset_device(void);

    
#endif

/* [] END OF FILE */
