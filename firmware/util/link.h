/*
 * link.h
 *
 *  Created on: 13 de fev de 2017
 *      Author: elder
 */


#ifndef LINK_H_
#define LINK_H_

#include "../include/floripasat_def.h"
/**
 * \fn decode
 * \brief This function translates a raw command data into a request packet structure
 * \param raw_package is the address of the start byte of the command data
 * \return a request packet, in request_packet_t structure
 *
 */
request_packet_t decode(uint8_t *raw_package);

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
uint32_t calculate_read_position(request_packet_t rqst_data_packet);

#endif /* LINK_H_ */
