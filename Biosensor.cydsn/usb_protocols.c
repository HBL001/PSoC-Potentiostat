/*********************************************************************************
* File Name: usb_protocols.c
*
* Description:
*  Source code for the protocols used by the USB.
*  converted to USBUART in January 2023
*
*
**********************************************************************************
 * Copyright Naresuan University, Phitsanulok Thailand 2017-2023
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include <project.h>
#include "usb_protocols.h"


/******************************************************************************
* Function Name: USB_CheckInput
*******************************************************************************
*
* Summary:
*  Check if any incoming USB data and store it to the input buffer
*
* Parameters:
*  uint8 buffer: array of where the data is stored
*
* Return:
*  true (1) if data has been inputed or false (0) if no data
*
*******************************************************************************/

uint8 USB_CheckInput(uint8 buffer[]) {
    
    uint16_t count = USBUART_GetCount();
    if ( count != 0 ) {
        USBUART_GetData(buffer, count);
        return true;
    }
    return false;
}

/******************************************************************************
* Function Name: USB_Export_Data
*************************************************************************************
*
* Summary:
*  Take a buffer as input and export it, the size of bytes to send is also inputted
*
* Parameters:
*  uint8 array array: array of data to export
*  uint16_t size: the number of bytes to send in the array
*
* Return:
*  None
*
*******************************************************************************************/

void USB_Export_Data(const char8 string[]) 
{
       
    while(USBUART_CDCIsReady() == 0)
        {
        }
   
    USBUART_PutString(string);
    
        while(USBUART_CDCIsReady() == 0)
        {
        }
        
    USBUART_PutCRLF();
        
    
}

/* [] END OF FILE */
