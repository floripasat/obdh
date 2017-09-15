/*
 * timer_b.c
 *
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * \file timer_b.c
 *
 * \brief Functions that deal with Timer_B
 *
 * \author Elder Tramontin
 *
 */

#include "timer_b.h"

void start_timer_b(void) {
    TBCTL = TBSSEL_1;   /**< use ACLK */
    TB0CCR0 = 32768;    /**< 32768 clocks until trigger a interrupt */
    TBCTL |= TBCLR;     /**< clear TBR */
    TBCTL |= TBIE;      /**< enable interrupt */
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
    __bic_SR_register_on_exit(LPM0_bits);   /**< exit from low power mode */
}

