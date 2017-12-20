/*
 * crc.c
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
 * \file crc.c
 * 
 * \brief Implementation of the CRC functions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/03/2017
 * 
 * \addtogroup crc
 * \{
 */

#include "crc.h"

uint8_t crc8(uint8_t initial_value, uint8_t polynomial, uint8_t *data, uint8_t len)
{
    uint8_t crc = initial_value;
    while(len--)
    {
        crc ^= *data++;
        uint8_t j = 0;
        for (j=0; j<8; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80)? polynomial: 0);
        }
        crc &= 0xFF;
    }
    
    return crc;
}

/*
uint16_t crc16_CCITT(uint16_t initial_value, uint8_t* data, uint8_t size)
{
    uint8_t x;
    uint16_t crc = initial_value;

    while(size--)
    {
        x = crc >> 8 ^ *data++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }
    
    return crc;
}
*/

//! \} End of crc implementation group
