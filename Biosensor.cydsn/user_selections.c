/*******************************************************************************
* File Name: user_selections.c
*
* Description:
*  Make some of functions the user can call through the USB. 
*  Only for the longer functions to keep main clearer
*
**********************************************************************************

 * Copyright Kyle Vitautas Lopin, Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include <project.h>
#include "stdio.h"  // gets rid of the type errors
#include "adc.h"
#include "user_selections.h"


/******************************************************************************
* Function Name: user_setup_TIA_ADC
*******************************************************************************
*
* Summary:
*  Set up the TIA and ADC circuit by changing the TIA resistor value, the ADC gain
*    setup if the user wants to use an external resistor
*  Parameters:
*  uint8 data_buffer[]: array of chars with the parameters to set the adc and tia components
*  input is A|U|X|Y|Z|W: where 
*  U is ADC configuration to use where config 1 uses a Vref of +-2.048 V and config 2 uses +-1.024 V
*  X - the TIA resistor value index, a string between 0-7 that sets the TIA resisot value
*  Y - the adc buffer gain setting
*  Z - T or F for if an external resistor is to be used and the AMux_working_electrode should be set according
*  W - 0 or 1 for which user resistor should be selected by AMux_working_electrode
*  
* Global variables: - found in calibrate.h and used by calibrate.c
*  uint8 TIA_resistor_value_index: index of whick TIA resistor to use, Supplied by USB input
*  uint8 ADC_buffer_index: which ADC buffer is used, gain = 2**ADC_buffer_index
*
* Return:
*  array of 20 bytes is loaded into the USB in endpoint (into the computer)
*
*******************************************************************************/
void user_setup_TIA_ADC(uint8_t data_buffer[]) {
    uint8_t adc_config = data_buffer[2]-'0';
    if (adc_config == 1 || adc_config == 2) {
        ADC_SigDel_SelectConfiguration(adc_config, DO_NOT_RESTART_ADC); 
    }
    TIA_resistor_value_index = data_buffer[4]-'0';
    if (TIA_resistor_value_index >= 0 || TIA_resistor_value_index <= 7) {
        TIA_SetResFB(TIA_resistor_value_index);  // see TIA.h for how the values work, basically 0 - 20k, 1 -30k, etc.
    }
    ADC_buffer_index = data_buffer[6]-'0';
    if (ADC_buffer_index >= 0 || ADC_buffer_index <= 3) {
        ADC_SigDel_SetBufferGain(ADC_buffer_index); 
    }
    if (data_buffer[8] == 'T') {
        tia_mux.use_extra_resistor = true;
        tia_mux.user_channel = data_buffer[10]-'0';  // not used yet
        data_buffer[8] = 0;
        AMux_TIA_resistor_bypass_Connect(0);
    }
    else {
        AMux_TIA_resistor_bypass_Disconnect(0);
    }
}


/******************************************************************************
* Function Name: user_voltage_source_funcs
*******************************************************************************
*
* Summary:
*  Check if the user wants to read what type of DAC is to be used or the user can set the DAC to be used

* Parameters:
*  uint8 data_buffer[]: array of chars used to setup the DAC or to read the DAC settings
*  input is VXY: where X is either 'R' or 'S' for read or set
*  Y is '0' or '1'
*
* Return:
*  export the DAC information to the USB, or set the DAC source depending on user input
*
*******************************************************************************/
void user_voltage_source_funcs(uint8_t data_buffer[]) {
    if (data_buffer[1] == 'R') {  // User wants to read status of VDAC
        uint8_t export_array[2];
        export_array[0] = 'V';
        export_array[1] = helper_check_voltage_source();
        USB_Export_Data(export_array, 2);
    }
    else if (data_buffer[1] == 'S') {  // User wants to set the voltage source
        helper_set_voltage_source(data_buffer[2]-'0');
        dac_Start();
        dac_Sleep();
    }
}

/******************************************************************************
* Function Name: user_reset_device
*******************************************************************************
*
* Summary:
*  Stop all operations by disabling all the isrs and reset the look up table index to 0
*  
* Global variables:
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void user_reset_device(void) {
    isr_dac_Disable();
    isr_adc_Disable();
    isr_adcAmp_Disable();
    helper_HardwareSleep();
}

/******************************************************************************
* Function Name: user_identify
*******************************************************************************
*
* Summary:
*  identify the device and test if the usb is working properly, this is what the program sends at the beginning
*  disable all interruptsincase the program has restarted so the device will also reset
*
* Parameters:
*  None
*
* Return:
*  which version of the device is attached to the USB port
*
*******************************************************************************/

void user_identify(void) {
    isr_adcAmp_Disable();
    isr_adc_Disable();
    isr_dac_Disable();
    USB_Export_Data((uint8_t*)"PSTAT ", 7);
}

/******************************************************************************
* Function Name: user_set_isr_timer
*******************************************************************************
*
* Summary:
*  Set the PWM that is used as the isr timer
* 
* Parameters:
*  uint8 data_buffer[]: array of chars used to setup the DAC or to read the DAC settings
*  input is T|XXXXX
*  XXXXX - uint16_t with the number to put in the period register
*  the compare register will be loaded with XXXXX / 2
*  
* Return:
*  Set the compare and period register of the pwm 
*
*******************************************************************************/
void user_set_isr_timer(uint8_t data_buffer[]) {
    PWM_isr_Wakeup();
    uint16_t timer_period = adc_Convert2Dec(&data_buffer[2], 5);
    PWM_isr_WriteCompare(timer_period / 2);  // not used in amperometry run so just set in the middle
    PWM_isr_WritePeriod(timer_period);
    PWM_isr_Sleep();
}



/******************************************************************************
* Function Name: user_run_amperometry
*******************************************************************************
*
* Summary:
*  Start an amperometry experiment.  Turn on all the hardware required, set the dac,
*  initialize the look up table index that will be a point where to put the data points
*  in the adc buffer, and start the Delta Sigma ADC to start converting.
* 
* Parameters:
*  uint8 data_buffer[]: array of chars used to make the look up table
*  input is M|XXXX|YYYY
*  XXXX - uint16_t number to set the DAC value to so the electrodes are at the approriate voltage
*  YYYY - uint16_t of how many data points to collect in each ADC buffer before exporting the data
*  
* Global variables:
*  
* Return:
*  uint16_t - number of data points to collect in each ADC buffer before exporting the data
*
*******************************************************************************/
uint16_t user_run_amperometry(uint8_t data_buffer[]) {
    helper_HardwareWakeup();  
    if (!isr_adcAmp_GetState()) {  // enable isr if it is not already
        if (isr_dac_GetState()) {  // User selected to run amperometry but a CV is still running 
            isr_dac_Disable();
            isr_adc_Disable();
        }
    }
    uint16_t dac_value = adc_Convert2Dec(&data_buffer[2], 4);  // get the voltage the user wants and set the dac
    dac_Setvalue(dac_value);
    
    ADC_SigDel_StartConvert();
    CyDelay(15);
    uint16_t buffer_size_data_pts = adc_Convert2Dec(&data_buffer[7], 4);  // how many data points to collect in each adc channel before exporting the data
    isr_adcAmp_Enable();
    return buffer_size_data_pts;
}


/* [] END OF FILE */
