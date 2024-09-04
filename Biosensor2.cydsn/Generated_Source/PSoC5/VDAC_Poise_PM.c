/*******************************************************************************
* File Name: VDAC_Poise_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VDAC_Poise.h"

static VDAC_Poise_backupStruct VDAC_Poise_backup;


/*******************************************************************************
* Function Name: VDAC_Poise_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void VDAC_Poise_SaveConfig(void) 
{
    if (!((VDAC_Poise_CR1 & VDAC_Poise_SRC_MASK) == VDAC_Poise_SRC_UDB))
    {
        VDAC_Poise_backup.data_value = VDAC_Poise_Data;
    }
}


/*******************************************************************************
* Function Name: VDAC_Poise_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void VDAC_Poise_RestoreConfig(void) 
{
    if (!((VDAC_Poise_CR1 & VDAC_Poise_SRC_MASK) == VDAC_Poise_SRC_UDB))
    {
        if((VDAC_Poise_Strobe & VDAC_Poise_STRB_MASK) == VDAC_Poise_STRB_EN)
        {
            VDAC_Poise_Strobe &= (uint8)(~VDAC_Poise_STRB_MASK);
            VDAC_Poise_Data = VDAC_Poise_backup.data_value;
            VDAC_Poise_Strobe |= VDAC_Poise_STRB_EN;
        }
        else
        {
            VDAC_Poise_Data = VDAC_Poise_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VDAC_Poise_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  VDAC_Poise_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VDAC_Poise_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VDAC_Poise_ACT_PWR_EN == (VDAC_Poise_PWRMGR & VDAC_Poise_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VDAC_Poise_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VDAC_Poise_backup.enableState = 0u;
    }
    
    VDAC_Poise_Stop();
    VDAC_Poise_SaveConfig();
}


/*******************************************************************************
* Function Name: VDAC_Poise_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  VDAC_Poise_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VDAC_Poise_Wakeup(void) 
{
    VDAC_Poise_RestoreConfig();
    
    if(VDAC_Poise_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VDAC_Poise_Enable();

        /* Restore the data register */
        VDAC_Poise_SetValue(VDAC_Poise_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
