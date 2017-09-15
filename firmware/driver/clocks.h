/*
 * clock.c
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
 * \file clock.c
 *
 * \brief Configuration of the MSP430's clocks
 *
 * \author Matheus Frata
 *
 */

#ifndef DRIVER_CLOCKS_H_
#define DRIVER_CLOCKS_H_

#include "../hal/obdh_hal.h"

/**
 * MAIN_clocks_setup will configure the ACLK clock to be sourced by the REFO (32768 Hz).
 * And the SMCLK to be sourced by the XT2 32 MHz Ext. Crystal. SMCLK outputs 16 MHz.
 * The MCLK also is sourced by the 32 MHz crystal with 1/2 prescaler (MCLK at 16MHz).
 */

#define XT_MAX_STARTUP_TIME 10000

/**
 * \fn clocks_setup
 *
 * \brief Configure the external crystals as clock sources, MCLK at 16MHz, SMCLK at 16MHz and ACLK at 32768Hz.
 * \return Success or fail
 */
uint8_t clocks_setup(void);


#endif /* DRIVER_CLOCKS_H_ */
