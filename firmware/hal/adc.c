#include "hal/adc.h"

void adc_setup_temp(void)
{
    //  Internal temperature reading setup
    REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to
                                            // ADC12_A ref control registers
    ADC12CTL0 = ADC12SHT0_15 | ADC12REFON | ADC12ON;
                                            // Internal ref = 1.5V
    ADC12CTL1 = ADC12SHP;                     // enable sample timer
    ADC12MCTL0 = ADC12SREF_1 | ADC12INCH_10;  // V(R+) = VREF+ and V(R-) = AVSS | ADC i/p ch A10 = temp sense i/p
    __delay_cycles(2000);   // Allow ~100us (at default UCS settings)
                                            // for REF to settle
    ADC12CTL0 |= ADC12ENC;
}

void adc_setup_A4(void)
{
    /*  A4 (PINO 1) VCC/2 ADC  (ADC 2)*/
    P6SEL |= 1<<4;                            // Enable A/D channel A0
    REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to
                                            // ADC12_A ref control registers
    ADC12CTL0 = ADC12ON|ADC12SHT02|ADC12REFON|ADC12REF2_5V;
                                            // Turn on ADC12, Sampling time
                                            // On Reference Generator and set to
                                            // 2.5V
    ADC12CTL1 = ADC12SHP;                     // Use sampling timer

    ADC12CTL2 = ADC12RES_2;
    ADC12CTL2 &= ~ADC12DF;
    ADC12MCTL0 = ADC12SREF_1| ADC12INCH_4; ;                 // Vr+=Vref+ and Vr-=AVss

    __delay_cycles(2000);                  // Delay for reference start-up

    ADC12CTL0 |= ADC12ENC;
}

uint16_t adc_read(void)
{
    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 |= ADC12SC;
    while (!(ADC12IFG & BIT0));
    return ADC12MEM0;

}
