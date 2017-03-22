#include <driver/adc.h>

void adc_setup(void) {
    //  Internal temperature reading setup
    REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to
                                            // ADC12_A ref control registers
    ADC12CTL0 = ADC12MSC | ADC12SHT0_15 | ADC12REFON | ADC12ON;

    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;                     // enable sample timer

    #if HAL_VERSION == HAL_V2_0
//    P6SEL |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
    BIT_SET(ADC_SEL, (X_SUNSEN_ADC_PIN | Y_SUNSEN_ADC_PIN | Z_SUNSEN_ADC_PIN
                      | OBDH_CURRENT_ADC_PIN | ADC_Vref_TEMP_PIN | VCC_3V3_PIN));

    ADC12MCTL0 = ADC12SREF_2 | ADC12INCH_0;                 // Vr+=Veref+ (3V) and Vr-=AVss
    ADC12MCTL1 = ADC12SREF_2 | ADC12INCH_1;
    ADC12MCTL2 = ADC12SREF_2 | ADC12INCH_2;
    ADC12MCTL3 = ADC12SREF_2 | ADC12INCH_3;
    ADC12MCTL4 = ADC12SREF_2 | ADC12INCH_4;
    ADC12MCTL5 = ADC12SREF_2 | ADC12INCH_5;
    ADC12MCTL6 = ADC12EOS | ADC12SREF_2 | ADC12INCH_10;     // Temp sensor
    #endif

    #if HAL_VERSION == HAL_V2_1
//    P6SEL |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
    BIT_SET(ADC_SEL, (X_SUNSEN_ADC_PIN | Y_SUNSEN_ADC_PIN | Z_SUNSEN_ADC_PIN
                      | OBDH_CURRENT_ADC_PIN | VCC_3V3_PIN));

    ADC12MCTL0 = ADC12SREF_2 | ADC12INCH_0;                 // Vr+=Veref+ (3V) and Vr-=AVss
    ADC12MCTL1 = ADC12SREF_2 | ADC12INCH_1;
    ADC12MCTL2 = ADC12SREF_2 | ADC12INCH_2;
    ADC12MCTL3 = ADC12SREF_2 | ADC12INCH_3;
    ADC12MCTL4 = ADC12SREF_2 | ADC12INCH_4;
    ADC12MCTL5 = ADC12EOS | ADC12SREF_2 | ADC12INCH_10;     // Temp sensor
    #endif

    __delay_cycles(2000);   // Allow ~100us (at default UCS settings)
                                            // for REF to settle
    ADC12CTL0 |= ADC12ENC;
}


uint16_t adc_read(uint8_t channel){
    while(ADC12CTL1 & ADC12BUSY);
    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 |= ADC12SC;

    switch(channel){
    case 0:
        while (!(ADC12IFG & BIT0));
        return ADC12MEM0;
    case 1:
        while (!(ADC12IFG & BIT1));
        return ADC12MEM1;
    case 2:
        while (!(ADC12IFG & BIT2));
        return ADC12MEM2;
    case 3:
        while (!(ADC12IFG & BIT3));
        return ADC12MEM3;
    case 4:
        while (!(ADC12IFG & BIT4));
        return ADC12MEM4;
    case 5:
        while (!(ADC12IFG & BIT5));
        return ADC12MEM5;
    case 6:
        while (!(ADC12IFG & BIT6));
        return ADC12MEM6;
    case 7:
        while (!(ADC12IFG & BIT7));
        return ADC12MEM7;
    case 8:
        while (!(ADC12IFG & BIT8));
        return ADC12MEM8;
    case 9:
        while (!(ADC12IFG & BIT9));
        return ADC12MEM9;
    case 10:
        while (!(ADC12IFG & BITA));
        return ADC12MEM10;
    case 11:
        while (!(ADC12IFG & BITB));
        return ADC12MEM11;
    case 12:
        while (!(ADC12IFG & BITC));
        return ADC12MEM12;
    case 13:
        while (!(ADC12IFG & BITD));
        return ADC12MEM13;
    case 14:
        while (!(ADC12IFG & BITE));
        return ADC12MEM14;
    case 15:
        while (!(ADC12IFG & BITF));
        return ADC12MEM15;
    }
    return 0;
}
