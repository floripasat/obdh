/*
 * eps.h
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
 * \file eps.h
 *
 * \brief This file deals with the EPS module
 *
 * \author Mario Baldini
 *
 */

#ifndef INTERFACE_EPS_H_
#define INTERFACE_EPS_H_

#include "../driver/i2c.h"
#include "../include/floripasat_def.h"
#include "../util/crc.h"

#define EPS_REQUEST_DATA_CMD    0x0F      /**< An arbitrary value was choosen                                           */
#define EPS_CHARGE_RESET_CMD    0xC1      /**< Warn the EPS to enter in reset battery charge mode (arbitrary value too) */

#define EPS_TIMEOUT_ERROR       0x00
#define EPS_OK                  0x01
#define EPS_CRC_ERROR           0x02

/*
 *  EPS FRAME
 */
typedef struct {
    uint8_t eps_misc[26];
    uint8_t battery_monitor[21];
    uint8_t temperatures[21];
    uint8_t energy_level[1];
} eps_package_t;

#define EPS_PACKAGE_LENGTH  sizeof(eps_package_t) + 7

void eps_setup(void);

/**
 * \fn eps_read
 *
 * \brief Send a data request to EPS and read the response
 * \param the address of a struct to store EPS data
 * \return Success or fail
 */
uint8_t eps_read(eps_package_t *package);

/**
 * \fn send_command_charge_reset
 *
 * \brief Send the charge reset command to EPS
 * \return Success or fail
 */
uint8_t send_command_charge_reset(void);

#endif /* INTERFACE_EPS_H_ */
