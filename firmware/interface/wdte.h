/*
 * wdte.h
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
 * \file wdte.h
 *
 * \brief Functions that deal with external watchdog timer
 *
 * \author Elder Tramontin
 *
 */

#ifndef INTERFACE_WDTE_H_
#define INTERFACE_WDTE_H_

#include "../hal/obdh_hal.h"

/**
 * \fn wdte_setup
 *
 * \brief Configure the IO pins to deals with the external watchdog timer
 * \return None
 */
void wdte_setup(void);

/**
 * \fn wdte_reset_counter
 *
 * \brief Reset the external watchdog timer counter
 * \return None
 */
void wdte_reset_counter(void);

#endif /* INTERFACE_WDTE_H_ */
