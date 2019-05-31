/*
 * link.c
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
 * \file link.c
 *
 * \brief This file provides the functions to deal with the link
 *
 * \author Elder Tramontin
 *
 */

#include <link.h>

telecommand_t  decode_telecommand(uint8_t *raw_package, uint16_t len) {
    telecommand_t telecommand;

    // ID code
    telecommand.id = raw_package[0];

    // Source callsign
    telecommand.src_callsign[0] = raw_package[1];
    telecommand.src_callsign[1] = raw_package[2];
    telecommand.src_callsign[2] = raw_package[3];
    telecommand.src_callsign[3] = raw_package[4];
    telecommand.src_callsign[4] = raw_package[5];
    telecommand.src_callsign[5] = raw_package[6];
    telecommand.src_callsign[6] = raw_package[7];

    // Data
    uint16_t i = 0;
    for(i=0; i<len; i++)
    {
        telecommand.data[i] = raw_package[i+8];
    }

    // Data length in bytes
    telecommand.data_len = len-8;

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

