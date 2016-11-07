/*
 * obdh.c
 *
 *  Created on: 30 de mai de 2016
 *      Author: mario
 */

// Interface with internal components of the OBDH SoC

#include <obdh_internal.h>


float obdh_temp_convert(uint16_t usTempRaw){
	float fTemp = (float)((long)usTempRaw - CALADC12_15V_30C) * TEMP_COEFICIENT + 30.0f;
	return fTemp;
}

uint16_t obdh_temp_read(void){
    uint16_t usTempRaw;
    usTempRaw = adc_read();

    return usTempRaw;
}

void obdh_setup(void){

    TEMP_COEFICIENT = (85.0 - 30.0) / (CALADC12_15V_85C - CALADC12_15V_30C);
////	Internal temperature reading setup
//	  REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to
//	                                            // ADC12_A ref control registers
//	  ADC12CTL0 = ADC12SHT0_8 | ADC12REFON | ADC12ON;
//	                                            // Internal ref = 1.5V
//	  ADC12CTL1 = ADC12SHP;                     // enable sample timer
//	  ADC12MCTL0 = ADC12SREF_1 | ADC12INCH_10;  // ADC i/p ch A10 = temp sense i/p
//	  __delay_cycles(DELAY_1_MS_IN_CYCLES);   // Allow ~100us (at default UCS settings)
//	                                            // for REF to settle
//	  ADC12CTL0 |= ADC12ENC;
}


