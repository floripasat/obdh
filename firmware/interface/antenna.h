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
 * \brief This file deals with the antenna module
 *
 * \author Elder Tramontin
 *
 */

#ifndef INTERFACE_ANTENNA_H_
#define INTERFACE_ANTENNA_H_

#include "../driver/i2c.h"

#define     ANTENNA_NOT_DEPLOYED    0       /**< Value if antenna is not deployed yet */
#define     ANTENNA_DEPLOYED        1       /**< Value if antenna is deployed         */


/**
 * \fn antenna_deployment_routine
 *
 * \brief Send commands through I2C to try to deploy antennas
 * \return None
 */
void antenna_deployment_routine(void);

/**
 * \fn verify_deployment_status
 *
 * \brief Send a command through I2C to read the deploy switches to know if the antennas were deployed
 * \return None
 */
uint8_t verify_deployment_status(void);

#endif /* INTERFACE_ANTENNA_H_ */
