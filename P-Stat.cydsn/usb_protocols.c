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
* Function Name: USB_Send
*************************************************************************************
*
* Summary:
*  Take a buffer as input and export it, the size of bytes to send is also inputted
*
* Parameters:
*  uint8_t data buffer: array of data to send
*  uint16_t count: the size of the buffer
*
* Return:
*  None
*
*******************************************************************************************/


void USB_send (uint8_t data_buffer[], size_t count ) 
{
    if (count==0) {
        count = strlen((char*)data_buffer); }
                   
        /* Echo back */
        USBUART_PutData(data_buffer, count);
                  
        /* Terminate with Null */
        if (USBUART_BUFFER_SIZE == count){
            
            /* Wait until component is ready to send data to PC. */
            while (0u == USBUART_CDCIsReady())
                {
                }

                /* Send zero-length packet to PC. */
                USBUART_PutData(NULL, 0u);
            }
                    
        /* Send a crlf out */
        while (0u == USBUART_CDCIsReady())
           {
           }
           USBUART_PutCRLF();        
}


/* [] END OF FILE */
