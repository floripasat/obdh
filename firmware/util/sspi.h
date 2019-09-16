/*
 * sspi.h
 *
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina
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
 * \brief This file has the functions to use an SPI interface built in software
 *
 * \author Elder Tramontin
 *
 * \version 0.2.2
 *
 * \defgroup sspi SSPI
 * \ingroup util
 */

#ifndef UTIL_SSPI_H_
#define UTIL_SSPI_H_

#include <msp430.h>
#include <stdint.h>

#include "../hal/obdh_hal.h"

/**
 * \brief Initialization of GPIOs: SCLK, MOSI, STE as output and MISO as input. STE disabled.
 * \return None
 */
void sspi_setup( void );

/**
 * \brief Send a byte through the software SPI interface
 * \param byte_to_send is the 8-bit value that will be sent
 * \return the received byte of this transaction (full-duplex)
 */
uint8_t sspi_tx (uint8_t byte_to_send);

/**
 * \brief Send one or more bytes through software SPI interface
 * \param data is a pointer to the first byte of the data to send
 * \param data_bytes_length is the length, in bytes, of the data to send
 * \return None
 */
void sspi_tx_multiple(uint8_t *data, uint8_t data_bytes_length);

/**
 * \brief Receive a byte through the software SPI interface, sending a dummy byte (0xFF) to the slave
 * \return the received byte
 */
#define sspi_rx() sspi_tx(0xFF);

/**
 * \brief Receive one or more bytes through software SPI interface
 * \param data is a pointer to the first byte of the received buffer
 * \param data_bytes_length is the length, in bytes, of the data to be received
 * \return None
 */
void sspi_rx_multiple(uint8_t *data, uint8_t data_bytes_length);

#endif /* UTIL_SSPI_H_ */

//! \} End of sspi group
