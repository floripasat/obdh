/*
 * payload2.c
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
 * \file payload2.c
 *
 * \brief Interface to deals with the Payload 1
 *
 * \author Akkauam Pallaoro
 *
 */

#include "payload2.h"
#include "../util/fsp/fsp.h"

void payload2_setup(void) {

}

uint8_t payload2_read(payload2_downlink_t *pkt) {

    uint8_t data[FSP_PKT_MAX_LENGTH];
    uint8_t pkt_lenght;
    FSPPacket fsp_packet;
    uint8_t payload2_status = PAYLOAD2_OK;
    uint8_t fsp_status = 0;
    uint8_t i = 0;

    i2c_set_slave(PAYLOAD2_BASE_ADDRESS, PAYLOAD2_I2C_SLAVE_ADDRESS);  /**< set the slave address to be the EPS address */

    i2c_set_mode(PAYLOAD2_BASE_ADDRESS, RECEIVE_MODE);           /**< set to receive */

//    fsp_init(FSP_ADR_OBDH);

    /*
     * Receive the packet
     */
//    fsp_reset();

   if(i2c_receive(PAYLOAD2_BASE_ADDRESS, &pkt->type, NO_STOP) == I2C_FAIL)
   {
       payload2_status = PAYLOAD2_TIMEOUT_ERROR;
   }

//    for(i=0; fsp_status == FSP_PKT_NOT_READY && i<4; i++){
//       fsp_status = fsp_decode(data[i++], &fsp_packet);
//    }

//   if(fsp_status == FSP_PKT_NOT_READY)
//   {
//       pkt_lenght = fsp_packet.length + 7;
//
//       if(fsp_packet.length == sizeof(pkt->data.bitstream_status_replay )){
//           pkt->type = PAYLOAD2_BITSTREAM_STATUS_REPLAY;
//       }else if(fsp_packet.length == sizeof(pkt->data.ccsds_telemetry)){
//           pkt->type = PAYLOAD2_CCSDS_TELEMETRY;
//       }else{
//           pkt->type = PAYLOAD2_NO_PENDING_DATA;
//       }
//
//   }

   switch (pkt->type)
   {
   case PAYLOAD2_BITSTREAM_STATUS_REPLAY:
       if(i2c_receive_burst(PAYLOAD2_BASE_ADDRESS,(uint8_t*) &pkt->data, sizeof(pkt->data.bitstream_status_replay), NO_START) == I2C_FAIL)
          {
              payload2_status = PAYLOAD2_TIMEOUT_ERROR;
          }
       break;
   case PAYLOAD2_CCSDS_TELEMETRY:
       if(i2c_receive_burst(PAYLOAD2_BASE_ADDRESS, (uint8_t*) &pkt->data, sizeof(pkt->data.ccsds_telemetry), NO_START) == I2C_FAIL)
          {
              payload2_status = PAYLOAD2_TIMEOUT_ERROR;
          }
       break;
   case PAYLOAD2_NO_PENDING_DATA:
   default:
       i2c_receive(PAYLOAD2_BASE_ADDRESS,(uint8_t*) &pkt->data,NO_START);       //just to send a stop bit
       payload2_status = PAYLOAD2_NO_DATA;
       break;
   }
//   if(i2c_receive_burst(PAYLOAD2_BASE_ADDRESS, (uint8_t*) pkt->data + sizeof(pkt->data.ccsds_telemetry),
//                        sizeof(pkt->data.bitstream_status_replay)-sizeof(pkt->data.ccsds_telemetry),
//                        START_STOP) == I2C_FAIL)
//   {
//       payload2_status = PAYLOAD2_TIMEOUT_ERROR;
//   }

//   do {
//       fsp_status = fsp_decode(data[i++], &fsp_packet);
//   } while(fsp_status == FSP_PKT_NOT_READY);
//
//   if(fsp_status == FSP_PKT_READY) {
//       for(i=0; i<fsp_packet.length; i++) {
//           ((uint8_t*)&pkt->data)[i] = fsp_packet.payload[i];
//       }
//   }

//    if(payload2_status != PAYLOAD2_TIMEOUT_ERROR)
//        payload2_status = fsp_status;

    return payload2_status;
}

uint8_t payload2_write(payload2_uplink_t *pkt) {

    uint8_t pkt_lenght = 0;
    uint8_t Buffer[PAYLOAD2_BITSTREAM_UPLOAD_LENGHT];
    FSPPacket fsp_packet;
    uint8_t payload2_status = PAYLOAD2_OK;

    fsp_init(FSP_ADR_OBDH);
    switch(pkt->type){
    case PAYLOAD2_CCSDS_TELECOMMAND:
        pkt_lenght = sizeof(pkt->data.ccsds_telecommand);
        break;
    case PAYLOAD2_BITSTREAM_UPLOAD:
        pkt_lenght = sizeof(pkt->data.bitstream_upload);
        break;
    case PAYLOAD2_BITSTREAM_SWAP:
    case PAYLOAD2_BITSTREAM_STATUS_REQUEST:
        pkt_lenght = 1;
//        pkt->data.cmd = pkt->type;
        break;
    default:
        return PAYLOAD2_INVALID_COMMAND;
    }

    //fsp_gen_data_pkt((uint8_t*)&pkt->data, pkt_lenght, FSP_ADR_PAYLOAD2,FSP_PKT_WITHOUT_ACK, &fsp_packet);
    //fsp_encode(&fsp_packet, Buffer, &pkt_lenght);

    if(i2c_send_burst(PAYLOAD2_BASE_ADDRESS, (uint8_t*) &pkt, pkt_lenght, START_STOP) == I2C_FAIL) {
        payload2_status = PAYLOAD2_TIMEOUT_ERROR;
    }

    return payload2_status;
}
