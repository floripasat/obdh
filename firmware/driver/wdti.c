/*
 * wdti.c
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
 * \file wdti.c
 *
 * \brief Functions that deal with the internal watchdog timer
 *
 * \author Mario Baldini
 *
 */

#include "wdti.h"

void wdti_setup(char mode, char time2trigger) {
    wdti_hold_counter();                                /**< Stop counting                                  */
    WDTCTL = WDTPW + WDTHOLD + mode + time2trigger;     /**< Configure the mode and interval until trigger  */
    if (mode == INTERVAL)
        SFRIE1 |= WDTIE;
    wdti_release_counter();                             /**< Restart counting                               */
}

void wdti_reset_counter(void){
    WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTCNTCL;      /**< counter = 0                                    */
}

void wdti_hold_counter(void){
    WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTHOLD;       /**< Stop the counting                              */
}

void wdti_release_counter(void){
    WDTCTL = ((WDTCTL & 0x00FF) + WDTPW) & ~WDTHOLD;    /**< Continue the counting                          */
}

void system_reboot(void){
    WDTCTL = 0x00;                                      /**< Force a reset                                  */
}

// Watchdog Timer interrupt service routine
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void){
    system_reboot();
}

