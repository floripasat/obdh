/*
 * rtc.c
 *
 *  Created on: 21 de jun de 2016
 *      Author: mfrata
 */

#include <driver/rtc.h>

void RTC_setup(void) {
    RTCCTL01 |= RTCRDYIE    //Real-time clock ready interrupt enable(every second)
               + RTCOFIE    //32-kHz crystal oscillator fault interrupt enable.
               + RTCHOLD;   //Real-time clock hold
    RTCSEC = 0x00;          //Start Seconds at Seconds = 0x00
    RTCMIN = 0x00;
    RTCHOUR = 0x00;
    RTCCTL01 &= ~(RTCHOLD); // Start RTC calendar mode
}
/******************************************************************************************/

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt void RTCISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_VECTOR))) RTCISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(RTCIV, 14)) {
    case 0:
        break;                           // Vector  0:  No interrupt
    case 2:                              // Vector  2:  RTCRDYIFG
        //reset sysclock_ms
        break;
    case 4:
        break;                           // Vector  4:  RTCEVIFG
    case 6:
        break;                           // Vector  6:  RTCAIFG
    case 8:
        break;                           // Vector  8:  RT0PSIFG
    case 10:
        break;                           // Vector 10:  RT1PSIFG
    case 12:
        break;                           // Vector 12:  RTCOFIFG
    case 14:
        break;                           // Vector 14:  Reserved
    default:
        break;
    }
}
