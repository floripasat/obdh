/*
 * link.h
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
 * \file link.h
 *
 * \brief This file provides the functions to deal with the link
 *
 * \author Elder Tramontin
 *
 */

#ifndef LINK_H_
#define LINK_H_

#include "../include/floripasat_def.h"
#include "../src/store_data_task.h"
/**
 * \brief This function translates a raw data into a telecommand structure
 * \param raw_package is the address of the start byte of the command data
 * \return a telecommand packet, in telecommand_t structure
 *
 */
telecommand_t decode_telecommand(uint8_t *raw_package, uint16_t len);

/**
 * \fn decode_request_data_telecommand
 * \brief This function translates a raw data into a request_data_packet_t structure
 * \param raw_package is the address of the start byte of the arguments data
 * \return a packet, in request_data_packet_t structure
 *
 */
request_data_packet_t  decode_request_data_telecommand(uint8_t *raw_package);

/**
 * \fn calculate_read_position
 * \brief This function reads the non-volatile memory pointers and, according the request packet calculates the read position
 * \param rqst_data_packet is the packet with the information about what are the requested packets
 * \return read position into the non-volatile memory
 */
uint32_t calculate_read_position(request_data_packet_t rqst_data_packet);

#endif /* LINK_H_ */
