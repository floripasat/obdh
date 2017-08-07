/*
 * temp_data.c
 *
 *  Created on: 25 de jul de 2017
 *      Author: André
 */

#include "temp_data.h"

void temperature_setup(void) {

    BIT_CLEAR(REFCTL0, REFMSTR);                        // Reset REFMSTR to hand over control to ADC12_A ref control registers
    ADC12CTL0 = ADC12SHT0_8 + ADC12REFON + ADC12ON;     // Internal ref = 1.5V
    ADC12CTL1 = ADC12SHP;                               // Enable sample timer
    ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;            // ADC i/p ch A10 = temp sense i/p
    __delay_cycles(100);                                // Delay to allow Ref to settle
    BIT_SET(ADC12CTL0, ADC12ENC);
}

uint16_t temperature_read(void) {

    uint16_t temperature;

    BIT_CLEAR(ADC12CTL0, ADC12SC);
    BIT_SET(ADC12CTL0, ADC12SC);                        // Sampling and conversion start

    while((ADC12IFG & ADC12IFG0) == 0);
    temperature = ADC12MEM0;
    
    return temperature;
}
