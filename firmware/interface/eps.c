/*
 * eps.c
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
 * \file eps.c
 *
 * \brief This file deals with the EPS module
 *
 * \author Mario Baldini
 *
 */

#include "eps.h"
#include "../util/fsp/fsp.h"

void eps_setup(void) {

}


uint8_t eps_read(eps_package_t *package) {
    uint8_t data[FSP_PKT_MAX_LENGTH];
    uint8_t cmd[10];
    uint8_t pkt_len;
    uint8_t *received_data = (uint8_t *)package;            /**< copy the address and work as a byte vector */
    uint8_t eps_status = EPS_OK;
    FSPPacket fsp_packet;
    uint8_t fsp_status = 0;
    uint8_t i = 0;

    i2c_set_slave(EPS_BASE_ADDRESS, EPS_I2C_SLAVE_ADRESS);  /**< set the slave address to be the EPS address */

    i2c_set_mode(EPS_BASE_ADDRESS, TRANSMIT_MODE);          /**< set to transmit */

    /*
     *  Send the request data command
     */
    fsp_init(FSP_ADR_OBDH);
    fsp_gen_cmd_pkt(FSP_CMD_SEND_DATA, FSP_ADR_EPS, FSP_PKT_WITHOUT_ACK, &fsp_packet);
    fsp_encode(&fsp_packet, cmd, &pkt_len);
    if(i2c_send_burst(EPS_BASE_ADDRESS, cmd, pkt_len, NO_STOP) == I2C_FAIL) {
        eps_status = EPS_TIMEOUT_ERROR;
    }

    i2c_set_mode(EPS_BASE_ADDRESS, RECEIVE_MODE);           /**< set to receive */

    /*
     * Receive the packet
     */
    fsp_reset();
    if(i2c_receive_burst(EPS_BASE_ADDRESS, data, EPS_PACKAGE_LENGTH, START_STOP) == I2C_FAIL) {
        eps_status = EPS_TIMEOUT_ERROR;
    }

    do {
        fsp_status = fsp_decode(data[i++], &fsp_packet);
    } while(fsp_status == FSP_PKT_NOT_READY);

    if(fsp_status == FSP_PKT_READY) {
        for(i=0; i<fsp_packet.length; i++) {
            received_data[i] = fsp_packet.payload[i];
        }
    }

    eps_status = fsp_status;

    return eps_status;
}

uint8_t send_command_charge_reset(void) {
    FSPPacket fsp_packet;
    uint8_t eps_pkt_len;
    uint8_t eps_pkt_cmd[8];
    uint8_t eps_status = EPS_OK;
    uint8_t rx_buffer[8];
    uint8_t fsp_status = FSP_PKT_NOT_READY;
    uint8_t i = 0;

    i2c_set_slave(EPS_BASE_ADDRESS, EPS_I2C_SLAVE_ADRESS);  /**< set the slave address to be the EPS address */

    i2c_set_mode(EPS_BASE_ADDRESS, TRANSMIT_MODE);          /**< set to transmit */

    /*
     *  Send the battery charge reset command
     */
    fsp_init(FSP_ADR_OBDH);
    fsp_gen_cmd_pkt(FSP_CMD_RESET_CHARGE, FSP_ADR_EPS, FSP_PKT_TYPE_CMD_WITH_ACK, &fsp_packet);
    fsp_encode(&fsp_packet, eps_pkt_cmd, &eps_pkt_len);

    if(i2c_send_burst(EPS_BASE_ADDRESS, eps_pkt_cmd, eps_pkt_len, NO_STOP) == I2C_FAIL) {     /**< send the bytes */
        eps_status = EPS_TIMEOUT_ERROR;
    }

    i2c_set_mode(EPS_BASE_ADDRESS, RECEIVE_MODE);           /**< set to receive */

    /*
     * Receive the packet
     */
    fsp_reset();
    if(i2c_receive_burst(EPS_BASE_ADDRESS, rx_buffer, FSP_PKT_MIN_LENGTH, START_STOP) == I2C_FAIL) {
        eps_status = EPS_TIMEOUT_ERROR;
    }
    else {
        /*
         * Verify the response, if not ACK this function is called again
         */
        do {
            fsp_status = fsp_decode(rx_buffer[i++], &fsp_packet);
        } while(fsp_status == FSP_PKT_NOT_READY);

        if(fsp_status == FSP_PKT_READY) {
            if(fsp_packet.type == FSP_PKT_TYPE_ACK) {
                eps_status = EPS_ACK;
            }
            if(fsp_packet.type == FSP_PKT_TYPE_NACK) {
                eps_status = EPS_NACK;
            }
        }
    }

    return eps_status;
}


