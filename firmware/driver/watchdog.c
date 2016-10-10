/*
 * watchdog.c
 *
 *  Created on: 11 de mai de 2016
 *      Author: mario
 */

#include <driver/watchdog.h>

void watchdog_setup(char mode, char time2trigger) {
	wdt_hold_counter();
	WDTCTL = WDTPW + WDTHOLD + mode + time2trigger;
	if (mode == INTERVAL)       //TODO add: WDTTMSEL = 1;
	    SFRIE1 |= WDTIE;            // Enable WDT interrupt
	wdt_release_counter();
}

void wdt_reset_counter(void){
	WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTCNTCL;
}

void wdt_hold_counter(void){
	WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTHOLD;
}

void wdt_release_counter(void){
	WDTCTL = ((WDTCTL & 0x00FF) + WDTPW) & ~WDTHOLD;
}

void system_reboot(void){
	WDTCTL = 0x00;
}

// Watchdog Timer interrupt service routine
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void){
	system_reboot();
}

