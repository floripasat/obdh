/*
 * payload_brave.c
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
 * \file payload_brave.c
 *
 * \brief Interface to deals with the Payload 1
 *
 * \author Akkauam Pallaoro
 *
 */

#include "payload_brave.h"
#include "../util/fsp/fsp.h"


uint8_t payload_brave_read(payload_brave_downlink_t *pkt) {

    uint8_t payload_brave_status = PAYLOAD_BRAVE_OK;

    i2c_set_slave(PAYLOAD_BRAVE_BASE_ADDRESS, PAYLOAD_BRAVE_I2C_SLAVE_ADDRESS);  /**< set the slave address to be the EPS address */

    i2c_set_mode(PAYLOAD_BRAVE_BASE_ADDRESS, RECEIVE_MODE);           /**< set to receive */


   if(i2c_receive(PAYLOAD_BRAVE_BASE_ADDRESS, &pkt->type, NO_STOP) == I2C_FAIL)
   {
       payload_brave_status = PAYLOAD_BRAVE_TIMEOUT_ERROR;
   }



   switch (pkt->type)
   {
   case PAYLOAD_BRAVE_BITSTREAM_STATUS_REPLAY:
       if(i2c_receive_burst(PAYLOAD_BRAVE_BASE_ADDRESS,(uint8_t*) &pkt->data, sizeof(pkt->data.bitstream_status_replay), NO_START) == I2C_FAIL)
       {
           payload_brave_status = PAYLOAD_BRAVE_TIMEOUT_ERROR;
       }
       break;
   case PAYLOAD_BRAVE_CCSDS_TELEMETRY:

       if(i2c_receive_burst(PAYLOAD_BRAVE_BASE_ADDRESS, (uint8_t*) &pkt->data, 120, NO_START | NO_STOP) == I2C_FAIL)
       {
           payload_brave_status = PAYLOAD_BRAVE_TIMEOUT_ERROR;
       }else if(i2c_receive_burst(PAYLOAD_BRAVE_BASE_ADDRESS, ((uint8_t*) &pkt->data) + 120, sizeof(pkt->data.ccsds_telemetry) - 120, NO_START) == I2C_FAIL)
       {
           payload_brave_status = PAYLOAD_BRAVE_TIMEOUT_ERROR;
       }
       break;
   case PAYLOAD_BRAVE_NO_PENDING_DATA:
   default:
       i2c_receive(PAYLOAD_BRAVE_BASE_ADDRESS,(uint8_t*) &pkt->data,NO_START);       //just to send a stop bit
       payload_brave_status = PAYLOAD_BRAVE_NO_DATA;
       break;
   }

    return payload_brave_status;
}

uint8_t payload_brave_write(payload_brave_uplink_t *pkt) {

    uint8_t pkt_lenght = 0;
    uint8_t payload_brave_status = PAYLOAD_BRAVE_OK;


    i2c_set_slave(PAYLOAD_BRAVE_BASE_ADDRESS, PAYLOAD_BRAVE_I2C_SLAVE_ADDRESS);  /**< set the slave address to be the EPS address */

    i2c_set_mode(PAYLOAD_BRAVE_BASE_ADDRESS, TRANSMIT_MODE);           /**< set to receive */


    switch(pkt->type){
    case PAYLOAD_BRAVE_CCSDS_TELECOMMAND:
        pkt_lenght = sizeof(pkt->data.ccsds_telecommand) +1; // +1 because sende the package type
        break;
    case PAYLOAD_BRAVE_BITSTREAM_UPLOAD:
        pkt_lenght = sizeof(pkt->data.bitstream_upload) +1;
        break;
    case PAYLOAD_BRAVE_BITSTREAM_SWAP:
    case PAYLOAD_BRAVE_BITSTREAM_STATUS_REQUEST:
        pkt_lenght = 1;
//        pkt->data.cmd = pkt->type;
        break;
    default:
        return PAYLOAD_BRAVE_INVALID_COMMAND;
    }

    if(i2c_send_burst(PAYLOAD_BRAVE_BASE_ADDRESS, (uint8_t*) pkt, pkt_lenght, START_STOP) == I2C_FAIL) {
        payload_brave_status = PAYLOAD_BRAVE_TIMEOUT_ERROR;
    }

    return payload_brave_status;
}
