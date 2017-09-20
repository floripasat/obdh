/*
 * antenna.h
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
 * \file antenna.h
 *
 * \brief This file verify the antennas' deployment status and do the antennas' deployment sequence
 *
 * \author Elder Tramontin
 *
 */

#ifndef INCLUDE_ANTENNA_H_
#define INCLUDE_ANTENNA_H_

#include <stdint.h>

#define ANTENNAS_NOT_DEPLOYED    0       /**< Value if antennas are not deployed yet */
#define ANTENNAS_DEPLOYED        1       /**< Value if antennas are deployed         */


/**
 * \fn antennas_deployment_routine
 *
 * \brief Send commands through I2C to try to deploy antennas
 * \return the antenna status: 1 = all antennas deployed. 0 = not all antennas deployed
 */
uint8_t antennas_deployment_routine(void);

/**
 * \fn verify_deployment_status
 *
 * \brief Read the deploy switches to know if the antennas were deployed
 * \return the antenna status: 1 = all antennas deployed. 0 = not all antennas deployed
 */
uint8_t verify_deployment_status(void);

#endif /* INCLUDE_ANTENNA_H_ */
