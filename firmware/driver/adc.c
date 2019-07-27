/*
 * adc.c
 *
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina
 *
 * This file is part of FloripaSat-OBDH.
 *
 * FloripaSat-OBDH is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FloripaSat-OBDH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief Functions of MSP430's ADC12 peripheral
 *
 * \author Elder Tramontin
 *
 * \version 0.2.6
 *
 * \addtogroup adc
 */

#include "../interface/debug/debug.h"

#include "adc.h"

void adc_setup(void) {
    debug_print_event_from_module(DEBUG_INFO, "ADC", "Initializing interface (Vref+ = 3.0V, Vref = GND)...");
    debug_new_line();

    // Internal temperature reading setup
    REFCTL0 &= ~REFMSTR;                                        // Reset REFMSTR to hand over control to ADC12_A ref control registers

    ADC12CTL0 = ADC12MSC | ADC12SHT0_15 | ADC12REFON | ADC12ON; // Vref+ = 3.0V, Vref- = 0V

    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;                       // enable sample timer

#if HAL_VERSION == HAL_V2_0
    BIT_SET(ADC_SEL, (X_SUNSEN_ADC_PIN | Y_SUNSEN_ADC_PIN | Z_SUNSEN_ADC_PIN |
                      OBDH_CURRENT_ADC_PIN | ADC_Vref_TEMP_PIN | VCC_3V3_PIN));

    ADC12MCTL0 = ADC12SREF_2 | ADC12INCH_0;                     // Sun sensor -X
    ADC12MCTL1 = ADC12SREF_2 | ADC12INCH_1;                     // Sun sensor -Y
    ADC12MCTL2 = ADC12SREF_2 | ADC12INCH_2;                     // Sun sensor -Z
    ADC12MCTL3 = ADC12SREF_2 | ADC12INCH_3;                     // OBDH current-sense
    ADC12MCTL4 = ADC12SREF_2 | ADC12INCH_4;                     // Vref temp. sensor
    ADC12MCTL5 = ADC12SREF_2 | ADC12INCH_5;                     // OBDH Voltage-sense
    ADC12MCTL6 = ADC12EOS | ADC12SREF_2 | ADC12INCH_10;         // Temperature sensor
#endif // HAL_VERSION

#if HAL_VERSION == HAL_V2_1
    BIT_SET(ADC_SEL, (X_SUNSEN_ADC_PIN | Y_SUNSEN_ADC_PIN | Z_SUNSEN_ADC_PIN |
                      OBDH_CURRENT_ADC_PIN | VCC_3V3_PIN));

    ADC12MCTL0 = ADC12SREF_2 | ADC12INCH_0;                     // Sun sensor -X
    ADC12MCTL1 = ADC12SREF_2 | ADC12INCH_1;                     // Sun sensor -Y
    ADC12MCTL2 = ADC12SREF_2 | ADC12INCH_2;                     // Sun sensor -Z
    ADC12MCTL3 = ADC12SREF_2 | ADC12INCH_3;                     // OBDH current-sense
    ADC12MCTL4 = ADC12SREF_2 | ADC12INCH_4;                     // OBDH Voltage-sense
    ADC12MCTL5 = ADC12EOS | ADC12SREF_2 | ADC12INCH_10;         // Temperature sensor
#endif // HAL_VERSION

    __delay_cycles(2000);                                       // Allow ~100us (at default UCS settings) for REF to settle
    ADC12CTL0 |= ADC12ENC;
}

uint16_t adc_read(uint8_t channel) {
    while(ADC12CTL1 & ADC12BUSY);
    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 |= ADC12SC;

    switch(channel) {
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

//! \} End of adc group
