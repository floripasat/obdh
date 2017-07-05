/*
 * timer_b.c
 *
 *  Created on: 22 de jun de 2017
 *      Author: elder
 */

#include "timer_b.h"

void start_timer_b(void) {
    TBCTL = TBSSEL_1;   /**< use ACLK */
    TB0CCR0 = 32768;    /**< 32768 clocks until trigger a interrupt */
    TBCTL |= TBCLR;      /**< clear TBR */
//    TBCTL |= TBIE;      /**< enable interrupt */
    TBCCTL0 = CCIE;     /**< enable interrupt */
    TBCTL |= MC_1;      /**< up mode -> counts up to 32768 */
}

void stop_timer_b(void) {
    TBCTL = 0;
    TBCCTL0 = 0;
}

// Timer B0 interrupt service routine
#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERB0_ISR (void)
{
    __bic_SR_register_on_exit(LPM0_bits);
}

