/*
 * decode.c
 *
 *  Created on: 13 de fev de 2017
 *      Author: elder
 */

#include "decode.h"

request_packet_t  decode(uint8_t *raw_package) {
    request_packet_t packet;
    packet.request_action = raw_package[0];
    packet.flags = ((uint16_t)raw_package[1]<<8) + (uint16_t)(raw_package[2]);
    packet.packages_count = raw_package[3];
    packet.packages_origin = raw_package[4];
    packet.packages_offset = ((int32_t)raw_package[5])<<24;
    packet.packages_offset += ((int32_t)raw_package[6])<<16;
    packet.packages_offset += ((int32_t)raw_package[7])<<8;
    packet.packages_offset += (int32_t)raw_package[8];

    
    return packet;
}

void shutdown(uint8_t *package) {
//    uint32_t seconds_to_wake_up = (uint32_t* package)[0];

}
