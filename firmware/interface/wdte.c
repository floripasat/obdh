/*
 * wdte.c
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
 * \file wdte.c
 *
 * \brief Functions that deal with external watchdog timer
 *
 * \author Elder Tramontin
 *
 */

#include <wdte.h>

void wdte_setup(void) {

    BIT_SET(WDI_EXT_DIR, WDI_EXT_PIN);     /**< Set as output pin */
    BIT_SET(MR_WDog_DIR, MR_WDog_PIN);     /**< Set as output pin */
	BIT_SET(MR_WDog_OUT, MR_WDog_PIN);     /**< Set output        */
}

void wdte_reset_counter(void) {

    BIT_TOGGLE(WDI_EXT_OUT, WDI_EXT_PIN);  /**< Toggle the pin to reset the counter */
}
