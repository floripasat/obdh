/*
 * link.c
 *
 *  Created on: 13 de fev de 2017
 *      Author: elder
 */

#include <link.h>

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

uint32_t calculate_read_position(request_packet_t rqst_data_packet) {
    uint32_t read_position;
    uint32_t last_read_pointer, last_write_pointer;

    last_read_pointer = get_last_read_pointer();
    last_write_pointer = get_last_write_pointer();

    if(rqst_data_packet.packages_origin == OLDER_PACKAGES_ORIGIN) {
        read_position =  last_read_pointer + rqst_data_packet.packages_offset;
    }
    else {
        read_position = last_write_pointer + rqst_data_packet.packages_offset;
    }

    if(read_position < last_read_pointer) { //TODO: incluir o caso do setor de leitura chegar a posição maxima da memoria
        read_position = last_read_pointer;
    }
    if(read_position > last_write_pointer) {
        read_position = last_write_pointer;
    }

    return read_position;
}


void shutdown() {
//TODO: shutdown all the transmissions (Tx).

}
