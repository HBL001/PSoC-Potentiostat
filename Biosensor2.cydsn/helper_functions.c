/*******************************************************************************
* File Name: helper_functions.c
*
* Description:
*  Functions used by main.
*  basically EEPROM functions at this point.
*
**********************************************************************************
* Copyright Highland Biosciences Ltd
* Copyright Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include <helper_functions.h>
#include <main.h>

/******************************************************************************
* Function Name: helper_Writebyte_EEPROM
*******************************************************************************
*
* Summary:
*    Start the eepromm, update the temperature and write a byte to it
*
* Parameters:
*     data: the byte to write in
*     address: the address to write the data at
*
* Return:
*
*******************************************************************************/

uint8_t helper_Writebyte_EEPROM(uint8_t data, uint16_t address) {
    EEPROM_Start();
    CyDelayUs(10);
    EEPROM_UpdateTemperature();
    uint8_t write_results = EEPROM_WriteByte(data, address);
    EEPROM_Stop();
    return write_results;
}

/******************************************************************************
* Function Name: helper_Readbyte_EEPROM
*******************************************************************************
*
* Summary:
*    Start the eepromm, update the temperature and read a byte to it
*
* Parameters:
*     address: the address to read the data from
*
* Return:
*     data that was read
*
*******************************************************************************/

uint8_t helper_Readbyte_EEPROM(uint16_t address) {
    EEPROM_Start();
    CyDelayUs(10);
    EEPROM_UpdateTemperature();
    CyDelayUs(10);
    uint8_t data = EEPROM_ReadByte(address);
    EEPROM_Stop();
    return data;
}

/******************************************************************************
* Function Name: helper_HardwareInit
*******************************************************************************
*
* Summary:
*    Start all the hardware needed for an experiment.
*
*******************************************************************************/
void helper_HardwareInit(void){ 
 
    /* setup the communications */
    Clock_PWM_Start();    
    LCD_Start();
    
    /* Initialise system timing */
    PWM_S_Init();
    PWM_M_Init();
    
    
    /* Voltage Control Circuit */  
    VDAC_Poise_Init();   
    Opamp_Aux_Init();
    
    AMux_electrode_Init();
    
   
    /* Current Measuring Circuit */
    ADC_DelSig_Init();
    VDAC_TIA_Init();
    TIA_Init();
    IDAC_calibrate_Init();
    AMux_TIA_input_Init();
  
    
}

/******************************************************************************
* Function Name: helper_Hardware Enable
*******************************************************************************
*
* Summary:
*    Start all the hardware needed for an experiment.
*
*******************************************************************************/
void helper_HardwareEnable(void){ 
 
    /* setup the communications and system timing */                             
    PWM_M_Enable();         
    PWM_S_Enable();
    
    /* Voltage Control Circuit */  
    VDAC_Poise_Enable();
    Opamp_Aux_Enable();
    
     /* Current Measuring Circuit */   
    ADC_DelSig_Enable();
    VDAC_TIA_Enable();
    TIA_Enable();
    AMux_TIA_input_Select(AMux_TIA_working_electrode_ch);    
}

/******************************************************************************
* Function Name: helper_HardwareSleep
*******************************************************************************
*
* Summary:
*    Put to sleep all the hardware needed for an experiment.
*
*******************************************************************************/
void helper_HardwareSleep(void){  
    
    /* Voltage Control Circuit */  
    VDAC_Poise_Sleep();
    Opamp_Aux_Sleep();
    
    /* Current Measuring Circuit */
//    ADC_SigDel_Sleep();
    VDAC_TIA_Sleep();
    TIA_Sleep();  
}


/******************************************************************************
* Function Name: helper_HardwareWakeup
*******************************************************************************
*
* Summary:
*    Start all the hardware needed for an experiment.
*
*******************************************************************************/
void helper_HardwareWakeup(void){  // wakeup all the components that have to be on for a reading
        
    /* Voltage Control Circuit */  
    
    VDAC_Poise_Wakeup();
    Opamp_Aux_Wakeup();
    
    /* Current Measuring Circuit */
   
    VDAC_TIA_Wakeup();
    TIA_Wakeup();  
        
}


/******************************************************************************
* Function Name: helper_WipeLCD
*******************************************************************************
*
* Summary: Wipe the LCD buffers ready for new messages
*
*******************************************************************************/

void helper_WipeLCD(void){  // wipe the text 
    memset(LCD_str_top, 0, MAX_LCD_BYTES);          
    memset(LCD_str_bot, 0, MAX_LCD_BYTES);              
}

void helper_WipeLCD_top(void){  // wipe the text top line
    memset(LCD_str_top, 0, MAX_LCD_BYTES);          
}

void helper_WipeLCD_bot (void){  // wipe the text top line
    memset(LCD_str_bot, 0, MAX_LCD_BYTES);          
}


/******************************************************************************
* Function Name: helper_ClrTx
*******************************************************************************
*
* Summary:
*    Wipe the buffers used to collect output from Host PC
*
*******************************************************************************/
void helper_ClrTx(void)
{        
    memset(TX_Data, 0, USBUART_BUFFER_SIZE);           
}


/******************************************************************************
* Function Name: helper_WipeIn
*******************************************************************************
*
* Summary:
*    Wipe the buffers used to collect output from Host PC
*
*******************************************************************************/
void helper_ClrRx(void)
{                   
    memset(RX_Data, 0, USBUART_BUFFER_SIZE);           
}


/* [] END OF FILE */
