/*
 * wdte.c
 *
 *  Created on: 23 de jan de 2017
 *      Author: elder
 */
#include <wdte.h>

void wdte_setup(void) {

	WDI_EXT_DIR |= WDI_EXT_PIN;
	MR_WDog_DIR |= MR_WDog_PIN;
	MR_WDog_OUT |= MR_WDog_PIN;

}

void wdte_reset_counter(void) {
#ifndef _DEBUG
    WDI_EXT_OUT ^= WDI_EXT_PIN;
#endif
}
