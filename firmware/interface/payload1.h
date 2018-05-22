/*
 * payload1.h
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
 * \file payload1.h
 *
 * \brief Interface to deals with the Payload 1
 *
 * \author Elder Tramontin
 *
 */

#ifndef PAYLOAD1_INTERFACE_H_
#define PAYLOAD1_INTERFACE_H_

#include "../include/floripasat_def.h"
#include "../driver/i2c.h"


#define PAYLOAD1_COMM_ERROR     0x00    /**< value when communication with payload is in error */
#define PAYLOAD1_POWER_ON       0x01    /**< value when communication with payload is "alive"  */
#define PAYLOAD1_POWER_OFF      0x02    /**< value when payload is powered off                 */

#define PAYLOAD1_DATA_LENGTH    100     /**< 100 bytes */

/**
 * \fn payload1_setup
 *
 * \brief Initialize the communication with payload
 * \return None
 */
void payload1_setup(void);

/**
 * \fn payload1_read
 *
 * \brief Read a packet of data from payload
 * \param data is a pointer where will be stored the read data
 * \param address is the payload address from the data will be read
 * \param bytes is the length, in bytes, of the read data
 * \return if the read was successful or failed
 */
uint8_t payload1_read(uint8_t* data, uint32_t address, uint8_t bytes);

/**
 * \fn payload1_write
 *
 * \brief Write a packet of data from payload
 * \param data is a pointer of the data to be written
 * \param address is the payload address where the data will be write
 * \param bytes is the length, in bytes, of the data to write
 * \return if the write was successful or failed
 */
uint8_t payload1_write(uint8_t* data, uint32_t address, uint8_t bytes);
#endif /* PAYLOAD1_INTERFACE_H_ */
