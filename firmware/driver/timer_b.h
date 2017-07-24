/*
 * timer_b.h
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
 * \file timer_b.h
 *
 * \brief Functions that deal with Timer_B
 *
 * \author Elder Tramontin
 *
 */

#ifndef DRIVER_TIMER_B_H_
#define DRIVER_TIMER_B_H_

#include <msp430.h>

/**
 * \fn start_timer_b
 *
 * \brief Configure timer B to overflow once each second.
 * \return None
 */
void start_timer_b(void);

/**
 * \fn stop_timer_b
 *
 * \brief Stop counting timer B and disable it interrupt
 * \return None
 */
void stop_timer_b(void);

#endif /* DRIVER_TIMER_B_H_ */
