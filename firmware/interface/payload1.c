/*
 * payload1.c
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
 * \file payload1.c
 *
 * \brief Interface to deals with the Payload 1
 *
 * \author Elder Tramontin
 *
 */

#include "payload1.h"

void payload1_setup(void) {
    /**< Set the enable payload pin as output */
    BIT_SET(PAYLOAD_0_EN_DIR, PAYLOAD_0_EN_PIN);
}

void payload1_power_state(uint8_t selector, uint8_t new_power_state) {
    uint8_t payload_status;

    if (selector == PAYLOAD_BOARD) {
        if (new_power_state == TURN_ON) {
            BIT_SET(PAYLOAD_0_EN_OUT, PAYLOAD_0_EN_PIN);
        }
        else {
            BIT_CLEAR(PAYLOAD_0_EN_OUT, PAYLOAD_0_EN_PIN);
        }
    }

    if (selector == PAYLOAD_FPGA) {
        /**< Figure out the current state */
        payload1_read(&payload_status, REG_STATUS, 1);

        /**< Set/clear only the fpga power bit */
        if (new_power_state == TURN_ON) {
            BIT_SET(payload_status, STATUS_POWER_MASK);
        }
        else {
            BIT_CLEAR(payload_status, STATUS_POWER_MASK);
        }

        /**< Send the new fpga power state */
        payload1_read(&payload_status, REG_STATUS, 1);
    }

    payload1_delay_ms(500);

    //TODO: take care of do a "turn off protocol"
}

void payload1_experiment_prepare(void) {
    //write in REG_DEBUGEN -> DEBUGEN_DISABLE_KEY
    //get OBDH current time and send to payload
    //write in REG_STATUS -> STATUS_SCRATCH_1_MASK
    //give a delay of 500ms before continue
}



uint8_t payload1_read(uint8_t* data, uint32_t address, uint8_t bytes) {  
    uint8_t payload1_status = PAYLOAD1_POWER_ON;

    i2c_set_slave(PAYLOAD1_BASE_ADDRESS, PAYLOAD1_I2C_SLAVE_ADDRESS);   /**< set the slave address to be the Payload1 address */

    i2c_set_mode(PAYLOAD1_BASE_ADDRESS, TRANSMIT_MODE);                 /**< set to transmit */

    /*
     * Send the Payload1 address to be read
     */
    if(i2c_send_burst(PAYLOAD1_BASE_ADDRESS, (uint8_t *) &address, 4, NO_STOP) == I2C_FAIL) {
        payload1_status = PAYLOAD1_COMM_ERROR;
    }

    i2c_set_mode(PAYLOAD1_BASE_ADDRESS, RECEIVE_MODE);                  /**< set to receive */

    /*
     * Read the data from the written address
     */
    if(i2c_receive_burst(PAYLOAD1_BASE_ADDRESS, data, bytes, START_STOP) == I2C_FAIL) {
        payload1_status = PAYLOAD1_COMM_ERROR;
    }

    return payload1_status;
}

uint8_t payload1_write(uint8_t* data, uint32_t address, uint8_t bytes) {
    uint8_t payload1_status = PAYLOAD1_POWER_ON;

    i2c_set_slave(PAYLOAD1_BASE_ADDRESS, PAYLOAD1_I2C_SLAVE_ADDRESS);

    i2c_set_mode(PAYLOAD1_BASE_ADDRESS, TRANSMIT_MODE);                 /**< set to transmit */

    /*
     * Send a Payload1 address
     */
    if(i2c_send_burst(PAYLOAD1_BASE_ADDRESS, (uint8_t *) &address, 4, NO_STOP) == I2C_FAIL) {
        payload1_status = PAYLOAD1_COMM_ERROR;
    }

    /*
     * Send a data to this address
     */
    if(i2c_send_burst(PAYLOAD1_BASE_ADDRESS, data, bytes, NO_START) == I2C_FAIL) {
        payload1_status = PAYLOAD1_COMM_ERROR;
    }

    return payload1_status;
}
