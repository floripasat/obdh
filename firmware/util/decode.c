/*
 * decode.c
 *
 *  Created on: 13 de fev de 2017
 *      Author: elder
 */

#include "decode.h"

request_data_packet_t*  decode(uint8_t *raw_package) {
    request_data_packet_t *packet = (request_data_packet_t *) raw_package;
    
    return packet;
}

void shutdown(uint8_t *package) {
//    uint32_t seconds_to_wake_up = (uint32_t* package)[0];

}
