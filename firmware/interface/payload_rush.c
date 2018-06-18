/*
 * payload_rush.c
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
 * \file payload_rush.c
 *
 * \brief Interface to deals with the payload
 *
 * \author Elder Tramontin
 *
 */

#include "payload_rush.h"

void rush_setup(void) {
    /**< Set the enable payload pin as output */
    BIT_CLEAR(PAYLOAD_RUSH_EN_OUT, PAYLOAD_RUSH_EN_PIN);
    BIT_SET(PAYLOAD_RUSH_EN_DIR, PAYLOAD_RUSH_EN_PIN);
}

void rush_power_state(uint8_t selector, uint8_t new_power_state) {
    uint8_t payload_status = 0;

    if (selector == PAYLOAD_BOARD) {
        if (new_power_state == TURN_ON) {
            BIT_SET(PAYLOAD_RUSH_EN_OUT, PAYLOAD_RUSH_EN_PIN);
        }
        else {
            BIT_CLEAR(PAYLOAD_RUSH_EN_OUT, PAYLOAD_RUSH_EN_PIN);
        }
    }

    if (selector == PAYLOAD_FPGA) {
        /**< Figure out the current state */
        rush_read(&payload_status, REG_STATUS, 1);

        /**< Set/clear only the fpga power bit */
        if (new_power_state == TURN_ON) {
            BIT_SET(payload_status, STATUS_POWER_MASK);
        }
        else {
            BIT_CLEAR(payload_status, STATUS_POWER_MASK);
        }

        rush_delay_ms(500);

        /**< Send the new fpga power state */
        rush_write(&payload_status, REG_STATUS, 1);
    }

    rush_delay_ms(500);

    //TODO: take care of do a "turn off protocol"
}

uint8_t rush_read(uint8_t* data, uint32_t address, uint16_t bytes) {
    uint8_t rush_status = RUSH_POWER_ON;

    i2c_set_slave(PAYLOAD_RUSH_BASE_ADDRESS, PAYLOAD_RUSH_I2C_SLAVE_ADDRESS);   /**< set the slave address to be the rush address */

    i2c_set_mode(PAYLOAD_RUSH_BASE_ADDRESS, TRANSMIT_MODE);                 /**< set to transmit */

    /*
     * Send the rush address to be read
     */
    if(i2c_send_burst(PAYLOAD_RUSH_BASE_ADDRESS, (uint8_t *) &address, 4, NO_STOP) == I2C_FAIL) {
        rush_status = RUSH_COMM_ERROR;
    }

    i2c_set_mode(PAYLOAD_RUSH_BASE_ADDRESS, RECEIVE_MODE);                  /**< set to receive */

    /*
     * Read the data from the written address
     */
    if(i2c_receive_burst(PAYLOAD_RUSH_BASE_ADDRESS, data, bytes, START_STOP) == I2C_FAIL) {
        rush_status = RUSH_COMM_ERROR;
    }

    return rush_status;
}

uint8_t rush_write(uint8_t* data, uint32_t address, uint16_t bytes) {
    uint8_t rush_status = RUSH_POWER_ON;

    i2c_set_slave(PAYLOAD_RUSH_BASE_ADDRESS, PAYLOAD_RUSH_I2C_SLAVE_ADDRESS);

    i2c_set_mode(PAYLOAD_RUSH_BASE_ADDRESS, TRANSMIT_MODE);                 /**< set to transmit */

    /*
     * Send a rush address
     */
    if(i2c_send_burst(PAYLOAD_RUSH_BASE_ADDRESS, (uint8_t *) &address, 4, NO_STOP) == I2C_FAIL) {
        rush_status = RUSH_COMM_ERROR;
    }

    /*
     * Send a data to this address
     */
    if(i2c_send_burst(PAYLOAD_RUSH_BASE_ADDRESS, data, bytes, NO_START) == I2C_FAIL) {
        rush_status = RUSH_COMM_ERROR;
    }

    return rush_status;
}
