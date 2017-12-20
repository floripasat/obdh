/*
 * crc.h
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
 * 
 * This file is part of FloripaSat-TTC.
 * 
 * FloripaSat-TTC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-TTC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-TTC.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \file crc.h
 * 
 * \brief CRC functions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/03/2017
 * 
 * \defgroup crc CRC
 * \ingroup beacon
 * \{
 */

#ifndef CRC_H_
#define CRC_H_

#include <stdint.h>

/**
 * \fn crc8
 * 
 * \brief crc8 checksum.
 * 
 * \param initial_value is the initial value of the crc8.
 * \param polynomial is the crc8 polynomial.
 * \param data is data to calculate the crc8.
 * \param len is lenght of the data.
 * 
 * \return The crc8 value of the given data.
 */
uint8_t crc8(uint8_t initial_value, uint8_t polynomial, uint8_t *data, uint8_t len);

/**
 * \fn crc16_CCITT
 * 
 * \brief Computes the crc16 value of an array of data.
 * 
 * \param initial_value is the initial value to compute the crc16 value.
 * \param data is the data to compute the crc16 value.
 * \param size is the length of the data array.
 * 
 * \return Returns the crc16 value of the data.
 */
//uint16_t crc16_CCITT(uint16_t initial_value, uint8_t* data, uint8_t size);

#endif // CRC_H_

//! \} End of crc group
