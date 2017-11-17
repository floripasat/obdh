/*
 * ttc.h
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
 * \file ttc.h
 *
 * \brief Interface to deals with TT&C module
 *
 * \author Elder Tramontin
 *
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
#define TTC_CMD_DATA_TRANSFER       0x80        /**< Warn the TT&C to receive a incoming data       */
#define TTC_CMD_SHUTDOWN            0x40        /**< Warn the TT&C to enter in shutdown mode        */
#define TTC_CMD_TX_MUTEX_REQUEST    0x20        /**< Request the use of downlink                    */
#define TTC_CMD_TX_MUTEX_RELEASE    0x10        /**< Release the use of downlink                    */

#define TTC_SHUTDOWN_ACK            0x01        /**< Expected response after a shutdown command     */

/*
 * TX mutex responses
 */
#define TTC_TX_FREE                 0x10        /**< The beacon is not being sent                   */
#define TTC_TX_BUSY                 0x11        /**< The beacon is being sent                       */


/**
 * \fn send_data_packet
 * \brief Encode and send the data packet with the fsp
 * \return None
 */
void send_data_packet();

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

/**
 * \fn ttc_send_data
 * \brief Send the data transfer command and the data
 * \return None
 */
void ttc_send_data();

/**
 * \fn ttc_send_mutex_request
 * \brief send a request to use the downlink
 * \return the TT&C response: the link can be free or busy
 */
uint8_t ttc_send_mutex_request();

/**
 * \fn ttc_tx_mutex_release
 * \brief send a command to signal the release of the downlink
 * \return None
 */
void ttc_tx_mutex_release();

/**
 * \fn ttc_send_shutdown
 * \brief send a command to warn the TT&C about a shutdown request
 * \return None
 */
void ttc_send_shutdown();

#endif /* INTERFACE_TTC_H_ */
