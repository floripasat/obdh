/*
 * link.h
 *
 *  Created on: 13 de fev de 2017
 *      Author: elder
 */


#ifndef LINK_H_
#define LINK_H_

#include "../include/floripasat_def.h"
#include "../src/store_data_task.h"
/**
 * \fn decode_telecommand
 * \brief This function translates a raw data into a telecommand structure
 * \param raw_package is the address of the start byte of the command data
 * \return a telecommand packet, in telecommand_t structure
 *
 */
telecommand_t decode_telecommand(uint8_t *raw_package);

/**
 * \fn decode_request_data_telecommand
 * \brief This function translates a raw data into a request_data_packet_t structure
 * \param raw_package is the address of the start byte of the arguments data
 * \return a packet, in request_data_packet_t structure
 *
 */
request_data_packet_t  decode_request_data_telecommand(uint8_t *raw_package);

/**
 * \fn shutdown
 * \brief This function may shutdown all the transmissions for 24 hours.
 */
void shutdown();

/**
 * \fn calculate_read_position
 * \brief This function reads the non-volatile memory pointers and, according the request packet calculates the read position
 * \param rqst_data_packet is the packet with the information about what are the requested packets
 * \return read position into the non-volatile memory
 */
uint32_t calculate_read_position(request_data_packet_t rqst_data_packet);

#endif /* LINK_H_ */
