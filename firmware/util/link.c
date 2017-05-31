/*
 * link.c
 *
 *  Created on: 13 de fev de 2017
 *      Author: elder
 */

#include <link.h>

telecommand_t  decode_telecommand(uint8_t *raw_package) {
    telecommand_t telecommand;

    telecommand.ID[0] = raw_package[0];
    telecommand.ID[1] = raw_package[1];
    telecommand.ID[2] = raw_package[2];
    telecommand.ID[3] = raw_package[3];
    telecommand.ID[4] = raw_package[4];
    telecommand.ID[5] = raw_package[5];

    telecommand.request_action = ((uint16_t)raw_package[7]<<8) + (uint16_t)(raw_package[6]);

    telecommand.arguments[0] = raw_package[8];
    telecommand.arguments[1] = raw_package[9];
    telecommand.arguments[2] = raw_package[10];
    telecommand.arguments[3] = raw_package[11];
    telecommand.arguments[4] = raw_package[12];
    telecommand.arguments[5] = raw_package[13];
    telecommand.arguments[6] = raw_package[14];
    telecommand.arguments[7] = raw_package[15];

    return telecommand;
}

request_data_packet_t  decode_request_data_telecommand(uint8_t *raw_package) {
    request_data_packet_t packet;

    packet.flags = ((uint16_t)raw_package[0]<<8) + (uint16_t)(raw_package[1]);
    packet.packages_count = raw_package[2];
    packet.packages_origin = raw_package[3];
    packet.packages_offset = ((int32_t)raw_package[4])<<24;
    packet.packages_offset += ((int32_t)raw_package[5])<<16;
    packet.packages_offset += ((int32_t)raw_package[6])<<8;
    packet.packages_offset += (int32_t)raw_package[7];

    return packet;
}

uint32_t calculate_read_position(request_data_packet_t rqst_data_packet) {
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
