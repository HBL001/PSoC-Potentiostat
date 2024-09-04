/*********************************************************************************
* File Name: main.c
* Version 1
*
* Description: Updated version for chronoamperometric testing
*
**********************************************************************************
* Some parts Copyright Highland Biosciences Ltd., Scotland
* Some parts Copyright Naresuan University, Phitsanulok Thailand
* Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*
* Refactored Aug 8, 2024
*
* PSoC-Potentiostat Commands
* The following are the inputs commands the device will take, all inputs are inputted as ASCII strings.
* 'I' - Identifies the device, will respond through the USB
* "L|X" - Set electrode configuration to 2 or 3 electrodes. X=0 is 2 electrodes, 1=3 electrodes
* "M|XXXX|YYYY" - Run an amperometry experiment. You need to start to read the data the device will start streaming when given this command. XXXX is an uint16 number to set the DAC value to so the electrodes are at the approriate voltage. YYYY is an uint16 of how many data points to collect in each ADC buffer before exporting the data
* 'B' - Calibrate the ADC and TIA signal chain.
* "T|XXXXX" - set the period value of the PWM used as a timer that starts the isrs to change the DAC and read the ADC. XXXXX is a uint16 that is put into the PWM that set the timing with a sample rate of 240 kHz / XXXXX
* "C|XXXXX" - set the compare value of the PWM used as a timer that sets when the DAC changes compared to when the ADC measures.
* 'X' - Reset the device. Disable all isrs and put the hardware to sleep.
* "D|XXX" - Set the voltage control 8 bit DAC. XXX is the value to put in the DAC
* 'H' - Wake up all the hardware, enable All
*********************************************************************************/

#include <main.h>
#include <helper_functions.h>


#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow usage of the floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
    asm (".global _printf_float");
#endif


uint8_t RX_Data[USBUART_BUFFER_SIZE];
uint8_t TX_Data[USBUART_BUFFER_SIZE];

uint8_t  LCD_str_top[MAX_LCD_BYTES];                       // buffer for LCD screen, make it extra big to avoid overflow
uint8_t  LCD_str_bot[MAX_LCD_BYTES];  

uint8_t TIA_resistor_value_index;
uint8_t ADC_buffer_index;
uint8_t ADC_config;


int16_t voltage;
double current;
uint8_t dac_Volts;
uint8_t conversionCount;
uint8_t dataReady;

double  measureADC;
uint32_t adcValue;
int32_t  signedValue;


// so this is called by PWM1 every 250mS
CY_ISR(ADC_ISR_RESET)
{
    conversionCount = 0;
    measureADC = 0;
}

CY_ISR(ADC_ISR)
{
    if (conversionCount < NUM_CONVERSIONS)    
    {
        if (ADC_DelSig_IsEndConversion(ADC_DelSig_RETURN_STATUS))
        {
            adcValue =  ADC_DelSig_GetResult32();
                                   
            if (adcValue & 0x80000000) 
            {
                signedValue = (int32_t)(adcValue - 0xFFFFFFFF - 1);
            } 
            else 
            {
                signedValue = (int32_t)adcValue;
            }
            
            measureADC = measureADC + ((double)(signedValue))/4;      
        
            conversionCount++;
        }
    }
    else
    {
        if (conversionCount == NUM_CONVERSIONS)
        {           
            dataReady = 1;
            conversionCount++;
        }
        else
        {
            /* do nothing */    
        }          
    }    
}
   

int main() {
       
    LCD_Start();
    
    // Enable global interrupts
    CyGlobalIntEnable;   
     
    /* Start USBFS operation with 5-V operation. */
    USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);
    
    /* initialise the system */
    helper_HardwareInit();  
    
    /* Safe Default Settings */
    AMux_electrode_Start();
    AMux_electrode_Select(TWO_ELECTRODE_CONFIG);  
        
    AMux_TIA_input_Start();
    AMux_TIA_input_Select(AMux_TIA_working_electrode_ch);   
    
    ADC_buffer_index = 1;
    ADC_config = 1;
    TIA_resistor_value_index = 0;
    TIA_SetResFB(TIA_resistor_value_index);
        
    dac_Volts = VIRTUAL_GROUND;
    dac_Setvalue(dac_Volts); 
    
    voltage = 0;
    
    /* enable the system ready to begin measurements */
    helper_HardwareEnable();  
    
    isr_adc_reset_StartEx(ADC_ISR_RESET);
    isr_adc_StartEx(ADC_ISR);
    
  
    ADC_DelSig_StartConvert(); // Start continuous conversions
    
    for(;;) 
    {
     
        /* Host can send double SET_INTERFACE request. */
        if (0u != USBUART_IsConfigurationChanged())
        {
            /* Initialize IN endpoints when device is configured. */
            if (0u != USBUART_GetConfiguration())
            {
                /* Enumeration is done, enable OUT endpoint to receive data from host. */
                USBUART_CDC_Init();
            }
        }
        
        
        
        /* Only service USB CDC when device is connected. */
        if (0u != USBUART_GetConfiguration())
        {
            /* Check for input data to process. */
            if (0u != USBUART_DataIsReady())
            {
                /* Read data into the buffer and re-enable OUT endpoint. */        
                helper_ClrRx();
                
                uint16 count = USBUART_GetAll(RX_Data);              
                                               
                if (0u != count) 
                {  
                    /* Wait until component is ready to send data to host. */
                    while (0u == USBUART_CDCIsReady())
                    {
                    }
                                       
                    /* Echo back */
                    USBUART_PutData(RX_Data, count);
                  
                     /* Terminate with Null */
                    if (USBUART_BUFFER_SIZE == count)
                    {
                        /* Wait until component is ready to send data to PC. */
                        while (0u == USBUART_CDCIsReady())
                        {
                        }

                        /* Send zero-length packet to PC. */
                        USBUART_PutData(NULL, 0u);
                    }
                    
                    /* Wait until component is ready to send data to PC. */
                    while (0u == USBUART_CDCIsReady())
                    {
                    }
                        
                    USBUART_PutCRLF();
                   
                   switch (RX_Data[0]) {                    // select case based upon the first 'L'etter
                
                    case CALIBRATE_TIA_ADC: ;               // 'B' calibrate the TIA / ADC current measuring circuit
                        calibrate_TIA();
                        break;
                               
                    case SET_ADC_CONFIG: ;                  // 'E' select one of the preset-configs
                        user_setup_ADC_CONFIG(RX_Data);
                        break;
                        
                    case TIA_RESISTOR: ;                    //  'F' select the TIA resistor value (index)
                        user_setup_TIA_RESISTOR(RX_Data);
                        break;
                                                
                    case ADC_BUFFER_GAIN: ;                 //  'G'  
                        user_setup_ADC_BUFFER_GAIN(RX_Data);      
                        break;

                    case RESET_DEVICE: ;                    // 'X' reset the device by disableing isrs
                        user_reset_device();
                        break;
                
                    case DEVICE_IDENTIFY: ;                 // 'I' identify the device 
                        user_identify();
                        break;
                
                    case CHANGE_NUMBER_ELECTRODES: ;        // 'L' User wants to change the electrode configuration         
                        user_setup_electrode(RX_Data);
                        break;
            
                    case SET_DAC_VALUE: ;                   // 'D' set the dac value, based on the voltage in millivolts
                        dac_Volts = dac_Volts2adc(RX_Data);
                        dac_Setvalue(dac_Volts);   
                        voltage = (int16_t)(dac_Volts-DAC_OFFSET)*DAC_RESOLUTION;           
                        break;
                
                    case STOP_HARDWARE: ;                   // 'M' stop data acquisition
                        helper_HardwareSleep();                 // Sleep the hardware
                        break;
                
                    case START_HARDWARE: ;                  // 'H' Start taking measurements from the electrode                
                        helper_HardwareWakeup();                // wake the hardware
                        break;
                    }    
                                      // end of switch statment                }
                }
            }
            else
            {   
                if (dataReady ==1) 
                {
                    /* update the LCD display */
                    LCD_ClearDisplay();
                    helper_WipeLCD();
                    
                    /* update line 1 */
                    LCD_Position(0u, 0u);
                    snprintf((char *)LCD_str_top,MAX_LCD_BYTES, "Voltage= %u mV", (int)voltage);
                    LCD_PrintString((const char*)LCD_str_top);                                       
                 
                    /* update line 2 */
                    LCD_Position(1u, 0u);
                    snprintf((char *)LCD_str_bot,MAX_LCD_BYTES, "Adc= %u A", (int) measureADC);
                    LCD_PrintString((const char*)LCD_str_bot);
                                       
                    /* Send out results to the USB Serial */
                    helper_ClrTx();
                    snprintf((char *)TX_Data, USBUART_BUFFER_SIZE, "Voltage= %u  mV ADC= %u mA",(int)voltage,(int) measureADC  );
                    USB_send (TX_Data, USBUART_BUFFER_SIZE ) ;
                    
                    dataReady =0;
                }        
            }
        }
    }                                                  
}  


/* [] END OF FILE */
