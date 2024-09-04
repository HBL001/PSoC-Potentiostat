/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#if !defined(MAIN_H)
#define MAIN_H

#include <project.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <globals.h>
    
#include <user_selections.h>
#include <calibrate.h>
#include <dac.h>
#include <helper_functions.h>
#include <adc.h>
#include <usb_protocols.h>
#include <globals.h>

    
/**************************************
*        Buffers
***************************************/   
extern uint8_t LCD_str_top[MAX_LCD_BYTES];
extern uint8_t LCD_str_bot[MAX_LCD_BYTES];
extern uint8_t RX_Data[USBUART_BUFFER_SIZE];
extern uint8_t TX_Data[USBUART_BUFFER_SIZE];

#endif
/* [] END OF FILE */
