/*
 * obdh.c
 *
 *  Created on: 30 de mai de 2016
 *      Author: mario
 */

// Interface with internal components of the OBDH SoC

#include <msp_internal.h>

const float CURR_COEF = (AVCC / (ADC_RANGE * RL_VALUE * CURRENT_GAIN * RSENSE_VALUE));

float obdh_temperature_convert(uint16_t temperature_raw){
	float temperature = (float)(((long)temperature_raw * 2 - CALADC12_15V_30C) * (85 - 30)) /
            (CALADC12_15V_85C - CALADC12_15V_30C) + 30.0f;
	return temperature;
}

uint16_t obdh_temperature_read(void){
    uint16_t temp_raw;
    temp_raw = adc_read(INTERNAL_TEMP_SENSOR_ADC_CH);

    return temp_raw;
}

float obdh_current_convert(uint16_t curr_raw){
    volatile float curr;
    curr = ((float)curr_raw) * CURR_COEF;

    return curr;
}

uint16_t obdh_current_read(void){
    uint16_t curr_raw;
    curr_raw = adc_read(OBDH_CURRENT_ADC_CH);

    return curr_raw;
}

float obdh_voltage_convert(uint16_t volt_raw){
    float volt = volt_raw * AVCC * VOLTAGE_DIVISOR / ADC_RANGE;
    return volt;
}

uint16_t obdh_voltage_read(void){
    uint16_t volt_raw;
    volt_raw = adc_read(VCC_3V3_ADC_CH);

    return volt_raw;
}

void obdh_setup(void){



    //CURRENT SENSING SETUP


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


