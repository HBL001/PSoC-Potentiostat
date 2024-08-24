/*******************************************************************************
* File Name: helper_functions.c
*
* Description:
*  Functions used by main.
*  basically EEPROM functions at this point.
*
**********************************************************************************
 * Copyright Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include "helper_functions.h"

/******************************************************************************
* Function Name: helper_check_voltage_source
*******************************************************************************
*
* Summary:
*  Look in the EEPROM to for what Voltage source is selected
  # TODO: dont look in eeprom first, check selected_voltage_source before going to the eeprom
*
* Parameters:
*
* Return:
*
* Global variables:
*  OUT_ENDPOINT:  number that is the endpoint coming out of the computer
*
*******************************************************************************/

uint8_t helper_check_voltage_source(void) {
    // start eeprom and read the value at voltage source address
    return helper_Readbyte_EEPROM(VDAC_ADDRESS);
}

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
* Function Name: helper_HardwareSetup
*******************************************************************************
*
* Summary:
*   Setup all the hardware needed for an experiment.  
*   Connect all the default analog muxes
*
*******************************************************************************/

void helper_HardwareSetup(uint8_t AMux_channel_select) {
    helper_HardwareStart();
    helper_HardwareSleep();

    AMux_electrode_Init();
    AMux_TIA_input_Init();
    AMux_TIA_resistor_bypass_Init();
    
    AMux_V_source_Init();
    
    AMux_TIA_resistor_bypass_Init();
    dac_Start();  // DAC has to be started after the AMux_V_source because it will set it based what DAC source is selected
    
    // initialise the analog muxes connections 
    AMux_electrode_Select(AMux_channel_select);             // start with 3 electrode configuration
    AMux_TIA_input_Select(AMux_TIA_working_electrode_ch);   // Connect the working electrode
    AMux_TIA_resistor_bypass_Select(0);                     // Start with no extra TIA resistor

}

/******************************************************************************
* Function Name: helper_HardwareStart
*******************************************************************************
*
* Summary:
*    Start all the hardware needed for an experiment.
*
*******************************************************************************/

void helper_HardwareStart(void){  // start all the components that have to be on for a reading
    ADC_SigDel_Start();
    TIA_Start();
    VDAC_TIA_Start();
    Opamp_Aux_Start();
    PWM_isr_Start();
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
    ADC_SigDel_Wakeup();
    TIA_Wakeup();
    VDAC_TIA_Wakeup();
    dac_Wakeup();
    CyDelay(1);
    Opamp_Aux_Wakeup();
    PWM_isr_Wakeup();
    
}

/******************************************************************************
* Function Name: helper_HardwareWakeup
*******************************************************************************
*
* Summary:
*    Put to sleep all the hardware needed for an experiment.
*
*******************************************************************************/

void helper_HardwareSleep(void){  // put to sleep all the components that have to be on for a reading
    ADC_SigDel_Sleep();
    dac_Sleep();
    TIA_Sleep();
    VDAC_TIA_Sleep();
    Opamp_Aux_Sleep();
    PWM_isr_Sleep();
    
}


/* [] END OF FILE */
