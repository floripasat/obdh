/*
 * decode.c
 *
 *  Created on: 13 de fev de 2017
 *      Author: elder
 */


#ifndef DECODE_H_
#define DECODE_H_

#include "../include/floripasat_def.h"

request_packet_t*  decode(uint8_t *raw_package);
void pack_data(uint8_t *package);
void shutdown(uint8_t *package);

#endif /* DECODE_H_ */
