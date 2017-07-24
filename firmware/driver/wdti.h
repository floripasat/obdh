/*
 * wdti.h
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
 * \file wdti.h
 *
 * \brief Functions that deal with the internal watchdog timer
 *
 * \author Mario Baldini
 *
 */

#ifndef DRIVER_WDTI_H_
#define DRIVER_WDTI_H_

#include <msp430.h>

/*
 * Watchdog timer operating mode
 */
#define INTERVAL            0x10
#define WATCHDOG            0x00

/*
 * Time to trigger
 */
#define WD_4_uSEC               0x07
#define WD_64_uSEC              0x06
#define WD_512_uSEC             0x05
#define WD_2_mSEC               0x27
#define WD_8_2_mSEC             0x04
#define WD_15_6_mSEC            0x26
#define WD_32_8_mSEC            0x03
#define WD_250_mSEC             0x25
#define WD_524_3_mSEC           0x02
#define WD_1_SEC                0x24
#define WD_8_4_SEC              0x01
#define WD_16_SEC               0x23
#define WD_134_SEC              0x00
#define WD_4_MIN_16_SEC         0x22
#define WD_8_MIN_16_SEC         0x21
#define WD_18_H_12_MIN_16_SEC   0x20


/**
 * \fn wdti_setup (char mode, char time2trigger)
 * That function setup the watchdog registers and start to count
 * \param mode is the operating mode (watchdog or timer)
 * \param time2trigger is the value that will be put into the registers
 * to count until some desired interval
 * \return None
 */
void wdti_setup(char mode, char time2trigger);

/**
 * \fn wdti_reset_counter(void)
 * That function resets the counter of the internal watchdog timer
 * \return None
 */
void wdti_reset_counter(void);

/**
 * \fn wdti_hold_counter(void)
 * That function stops the counting
 * \return None
 */
void wdti_hold_counter(void);

/**
 * \fn wdti_release_counter(void)
 * That function release the counting
 * \return None
 */
void wdti_release_counter(void);

/**
 * \fn system_reboot(void)
 * That function reset the microcontroller
 * \return None
 */
void system_reboot(void);


#endif /* DRIVER_WDTI_H_ */
