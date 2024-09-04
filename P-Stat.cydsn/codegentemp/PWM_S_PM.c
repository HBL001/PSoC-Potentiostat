/*******************************************************************************
* File Name: PWM_S_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_S.h"

static PWM_S_backupStruct PWM_S_backup;


/*******************************************************************************
* Function Name: PWM_S_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_S_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_S_SaveConfig(void) 
{

    #if(!PWM_S_UsingFixedFunction)
        #if(!PWM_S_PWMModeIsCenterAligned)
            PWM_S_backup.PWMPeriod = PWM_S_ReadPeriod();
        #endif /* (!PWM_S_PWMModeIsCenterAligned) */
        PWM_S_backup.PWMUdb = PWM_S_ReadCounter();
        #if (PWM_S_UseStatus)
            PWM_S_backup.InterruptMaskValue = PWM_S_STATUS_MASK;
        #endif /* (PWM_S_UseStatus) */

        #if(PWM_S_DeadBandMode == PWM_S__B_PWM__DBM_256_CLOCKS || \
            PWM_S_DeadBandMode == PWM_S__B_PWM__DBM_2_4_CLOCKS)
            PWM_S_backup.PWMdeadBandValue = PWM_S_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_S_KillModeMinTime)
             PWM_S_backup.PWMKillCounterPeriod = PWM_S_ReadKillTime();
        #endif /* (PWM_S_KillModeMinTime) */

        #if(PWM_S_UseControl)
            PWM_S_backup.PWMControlRegister = PWM_S_ReadControlRegister();
        #endif /* (PWM_S_UseControl) */
    #endif  /* (!PWM_S_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_S_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_S_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_S_RestoreConfig(void) 
{
        #if(!PWM_S_UsingFixedFunction)
            #if(!PWM_S_PWMModeIsCenterAligned)
                PWM_S_WritePeriod(PWM_S_backup.PWMPeriod);
            #endif /* (!PWM_S_PWMModeIsCenterAligned) */

            PWM_S_WriteCounter(PWM_S_backup.PWMUdb);

            #if (PWM_S_UseStatus)
                PWM_S_STATUS_MASK = PWM_S_backup.InterruptMaskValue;
            #endif /* (PWM_S_UseStatus) */

            #if(PWM_S_DeadBandMode == PWM_S__B_PWM__DBM_256_CLOCKS || \
                PWM_S_DeadBandMode == PWM_S__B_PWM__DBM_2_4_CLOCKS)
                PWM_S_WriteDeadTime(PWM_S_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_S_KillModeMinTime)
                PWM_S_WriteKillTime(PWM_S_backup.PWMKillCounterPeriod);
            #endif /* (PWM_S_KillModeMinTime) */

            #if(PWM_S_UseControl)
                PWM_S_WriteControlRegister(PWM_S_backup.PWMControlRegister);
            #endif /* (PWM_S_UseControl) */
        #endif  /* (!PWM_S_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_S_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_S_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_S_Sleep(void) 
{
    #if(PWM_S_UseControl)
        if(PWM_S_CTRL_ENABLE == (PWM_S_CONTROL & PWM_S_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_S_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_S_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_S_UseControl) */

    /* Stop component */
    PWM_S_Stop();

    /* Save registers configuration */
    PWM_S_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_S_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_S_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_S_Wakeup(void) 
{
     /* Restore registers values */
    PWM_S_RestoreConfig();

    if(PWM_S_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_S_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
