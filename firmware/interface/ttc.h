/*
 * ttc.h
 *
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
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
 * \brief Interface to deals with TT&C module
 *
 * \author Elder Tramontin
 */

#ifndef INTERFACE_TTC_H_
#define INTERFACE_TTC_H_

#include "../driver/i2c.h"
#include "../include/floripasat_def.h"
#include "../util/sspi.h"
#include "../util/fsp/fsp.h"

/*
 * Commands
 */
#define TTC_CMD_HIBERNATION         0x11        /**< Warn the TTC to enter in hibernation mode */
#define TTC_CMD_TX_MUTEX_REQUEST    0x22        /**< Request the use of downlink */

#define TTC_ACK                     FSP_PKT_WITH_ACK
#define TTC_NACK                    FSP_PKT_WITHOUT_ACK
/**
 * \fn send_data_packet
 * \brief Encode and send the data packet with the fsp
 * \return None
 */
void send_data_packet(void);

/**
 * \fn send_data_packet
 * \brief Encode and send the command packet with the fsp
 * \param command
 * \return None
 */
void send_command_packet(uint8_t command);

/**
 * \fn ttc_copy_data
 * \brief Store the data to be sent through beacon in a packet
 * \return the beacon data packet
 */
beacon_packet_t ttc_copy_data(void);


uint8_t receive_packet(uint8_t* received_packet, uint8_t payload_len);



#endif /* INTERFACE_TTC_H_ */
