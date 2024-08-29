/*********************************************************************************
* File Name: usb_protocols.h
*
* Description:
*  This file contains the function prototypes and constants used for
*  the USB protocols.
*
*
**********************************************************************************
* Copyright Highland Biosciences Ltd.
* Copyright Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/


#if !defined(USB_PROTOCOLS_H)
#define USB_PROTOCOLS_H
    
#include <project.h>
#include "globals.h"
   
    
/***************************************
*        Function Prototypes
***************************************/  
    
uint8_t USB_CheckInput(uint8_t buffer[]);
void USB_Export_Data(const char8 string[]);

#endif

/* [] END OF FILE */
