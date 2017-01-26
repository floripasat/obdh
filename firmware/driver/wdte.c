/*
 * wdte.c
 *
 *  Created on: 23 de jan de 2017
 *      Author: elder
 */
#include <wdte.h>

void wdte_setup(void){
    MR_WDog_DIR |= MR_WDog_PIN;
    MR_WDog_OUT |= MR_WDog_PIN;
    WDI_EXT_DIR |= WDI_EXT_PIN;
}

void wdte_reset_counter(void)
{
    WDI_EXT_OUT ^= WDI_EXT_PIN;
}
