/*
 * ttc.h
 *
 *  Created on: 09 de fev de 2017
 *      Author: elder
 */

#ifndef INTERFACE_TTC_H_
#define INTERFACE_TTC_H_

#include "../driver/i2c.h"
#include "../include/floripasat_def.h"
#include "../util/sspi.h"

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
 * \fn ttc_copy_data
 * \brief Store the data to be sent through beacon in a packet
 * \return the beacon data packet
 */
beacon_packet_t ttc_copy_data(void);

/**
 * \fn ttc_send_data
 * \brief Store the data to be sent to TT&C in a packet
 * \param ttc_packet is the address of the packet to be sent to TT&C
 * \return None
 */
void ttc_send_data(ttc_packet_t* ttc_packet);

/**
 * \fn ttc_send_mutex_request
 * \brief send a request to use the downlink
 * \return the TT&C response: the link can be free or busy
 */
uint8_t ttc_send_mutex_request(void);

/**
 * \fn ttc_tx_mutex_release
 * \brief send a command to signal the release of the downlink
 * \return None
 */
void ttc_tx_mutex_release(void);

/**
 * \fn ttc_send_shutdown
 * \brief send a command to warn the TT&C about a shutdown request
 * \return None
 */
void ttc_send_shutdown(void);

#endif /* INTERFACE_TTC_H_ */
