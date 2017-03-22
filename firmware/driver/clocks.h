/*
 * clocks.h
 *
 *  Created on: 21 de jun de 2016
 *      Author: mfrata
 */

#ifndef DRIVER_CLOCKS_H_
#define DRIVER_CLOCKS_H_

#include <msp430.h>
#include "../hal/obdh_hal.h"

/**
 *     MAIN_clocks_setup will configure the ACLK clock to be sourced by the REFO (32768 Hz).
 *  And the SMCLK to be sourced by the XT2 4 MHz Ext. Crystal. SMCLK outputs 1 MHz.
 *     The MCLK also is sourced by the 4 MHz crystal with no prescaler.
 */

#define XT_MAX_STARTUP_TIME 20000

//todo separate into one function to each clk
uint8_t clocks_setup(void);
void set_vcore_up (unsigned int level);
void setup_xt1_xt2(void);
uint8_t test_fault_flags(void);
void setup_clks(void);

#endif /* DRIVER_CLOCKS_H_ */
