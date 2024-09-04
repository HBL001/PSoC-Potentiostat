/*******************************************************************************
* File Name: calibrate.h
*
* Description:
*  This file contains the function prototypes and constants used for
*  the protocols to calibrate a TIA / delta sigma ADC with an IDAC
*
**********************************************************************************
 * Copyright Kyle Vitautas Lopin, Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#if !defined(CALIBRATE_H)
#define CALIBRATE_H

#include <project.h>
#include "cytypes.h"
#include <globals.h>
#include <math.h>
#include <stdio.h>
#include <usb_protocols.h>
#include <helper_functions.h>

    
/**************************************
*      IDAC vs ADC Comparison table
**************************************/
    
union calibrate_data_usb_union {
    uint8_t usb[4*Number_calibration_points];  // 2 * the number of data points
    int16_t data[2*Number_calibration_points];  // 2 * data points because 1 for the IDAC value and the other for the ADC result
};
union calibrate_data_usb_union calibrate_array;  // allocate space to put adc measurements

/* This union will save the calibration results and the IDAC values used in the format of
calibrate_data_usb_union = [highest IDAC value, 2nd highest IDAC value, 0 IDAC input, 2nd lowest IDAC value, 
lowest IDAC value, ADC reading for highest IDAC value, ADC reading for 2nd highest IDAC value,
ADC reading 0 IDAC input, ADC reading for 2nd lowest IDAC value, ADC reading for lowest IDAC value]
*/

extern int calibrate_TIA_resistor_list[8];
extern uint8_t TIA_resistor_value_index;


/***************************************
*        Function Prototypes
***************************************/  
void calibrate_TIA(void);
void Calibrate_Hardware_Wakeup(void);
void calibrate_step(uint16_t IDAC_value, uint8 IDAC_index);
void Calibrate_Hardware_Sleep(void); 

#endif
/* [] END OF FILE */
