/*********************************************************************************
* File Name: DAC.c
* Description: simple handler for the 8-BIT
*
**********************************************************************************
* Copyright Highland Biosciences Ltd.
* Copyright Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include "dac.h"
#include "globals.h"

/******************************************************************************
* Function Name: dac_Start
*******************************************************************************
*
* Summary:
*  Start the correct voltage source.  
*  Figure what source is being used, set the  correct AMux settings and start 
*  the correct source
*
* Parameters:
*
*  Global variables:
*  selected_voltage_source:  voltage source that is set to run, 
*
*******************************************************************************/

void dac_Start(void) 
{   
        VDAC_source_Start();
        dac_ground_value = VIRTUAL_GROUND / 16;  // value of dac to make 0 V across working and aux electrodes
}

/******************************************************************************
* Function Name: dac_Sleep
*******************************************************************************
* Summary:
*  Put to sleep the correct voltage source
*
*******************************************************************************/

void dac_Sleep(void) {
     VDAC_source_Sleep();   
}

/******************************************************************************
* Function Name: dac_Wakeup
*******************************************************************************
*
* Summary:
*  Wake up the correct voltage source
*
* Parameters:
*
*
* Global variables:
*
*******************************************************************************/

void dac_Wakeup(void) 
{
 VDAC_source_Wakeup();
}


/******************************************************************************
* Function Name: dac_Setvalue
*******************************************************************************
*
* Summary:
*  Set the value of voltage source
*
* Parameters:
*  uint16_t value: poise voltage (V)
*
* Global variables:
*
*******************************************************************************/

void dac_Setvalue(uint16_t value) {
    
    VDAC_source_SetValue(value);
   
}

/* [] END OF FILE */
