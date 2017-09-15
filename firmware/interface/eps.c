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

void eps_setup(void) {

}


uint8_t eps_read(eps_package_t *package) {
    uint8_t *data = (uint8_t *)package;         /**< copy the address and work as a byte vector */
    uint8_t eps_status = EPS_OK;

    i2c_set_slave(EPS_BASE_ADDRESS, EPS_I2C_SLAVE_ADRESS);  /**< set the slave address to be the EPS address */

    i2c_set_mode(EPS_BASE_ADDRESS, TRANSMIT_MODE);          /**< set to transmit */


    /*
     *  Send the request data command
     */
    if(i2c_send(EPS_BASE_ADDRESS, EPS_REQUEST_DATA_CMD, NO_STOP) == I2C_FAIL) {
        eps_status = EPS_TIMEOUT_ERROR;
    }

    i2c_set_mode(EPS_BASE_ADDRESS, RECEIVE_MODE);           /**< set to receive */

    /*
     * Receive the packet
     */
    if(i2c_receive_burst(EPS_BASE_ADDRESS, data, EPS_PACKAGE_LENGTH, START_STOP) == I2C_FAIL) {
        eps_status = EPS_TIMEOUT_ERROR;
    }

    if(eps_status == EPS_OK) {
        /*
         *  Evaluate the CRC8 and compare with the received value
         */
        if( crc8(CRC_SEED, CRC_POLYNOMIAL, package->msp430, EPS_PACKAGE_LENGTH-2) != package->crc_value){
            eps_status = EPS_CRC_ERROR;
        }
    }

    return eps_status;
}


