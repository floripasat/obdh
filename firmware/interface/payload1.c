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

}

uint8_t payload1_read(uint8_t* data, uint32_t address, uint8_t bytes) {  
    uint8_t payload1_status = PAYLOAD1_POWER_ON;      //set status to payload alive

    i2c_set_slave(PAYLOAD1_BASE_ADDRESS, PAYLOAD1_I2C_SLAVE_ADDRESS); //set to base adress the slave adress of RUSH (0x29)

    i2c_set_mode(PAYLOAD1_BASE_ADDRESS, TRANSMIT_MODE);   //change to Transmit Mode

    if(i2c_send_burst(PAYLOAD1_BASE_ADDRESS, (uint8_t *) &address, 4, NO_STOP) == I2C_FAIL) {  //< try to send a register address //
        payload1_status = PAYLOAD1_COMM_ERROR;  //set error case the communication between OBDH and Payload 1 fails
    }

    i2c_set_mode(PAYLOAD1_BASE_ADDRESS, RECEIVE_MODE); //change to Receive Mode

    
    if(i2c_receive_burst(PAYLOAD1_BASE_ADDRESS, data, bytes, START_STOP) == I2C_FAIL) { // try to read from the register address of Payload1
        payload1_status = PAYLOAD1_COMM_ERROR;      //set error case the communication between OBDH and Payload 1 fails
    }

    return payload1_status;  //return status of Payload = PAYLOAD1_POWER_ON or PAYLOAD1_COMM_ERROR
}

uint8_t payload1_write(uint8_t* data, uint32_t address, uint8_t bytes) {  //write on adrres
    uint8_t payload1_status = PAYLOAD1_POWER_ON;    //set status of Payload to Power On

    i2c_set_slave(PAYLOAD1_BASE_ADDRESS, PAYLOAD1_I2C_SLAVE_ADDRESS);   //set to base adress the slave adress of RUSH (0x29)

    i2c_set_mode(PAYLOAD1_BASE_ADDRESS, TRANSMIT_MODE);    //change to Transmit Mode

    if(i2c_send_burst(PAYLOAD1_BASE_ADDRESS, (uint8_t *) &address, 4, NO_STOP) == I2C_FAIL) {    // try to send a register address //
        payload1_status = PAYLOAD1_COMM_ERROR;  //set error case the communication between OBDH and Payload 1 fails
    }

    if(i2c_send_burst(PAYLOAD1_BASE_ADDRESS, data, bytes, NO_START) == I2C_FAIL) {  // try to read from the register address of Payload1
        payload1_status = PAYLOAD1_COMM_ERROR;  //set error case the communication between OBDH and Payload 1 fails
    }

    return payload1_status; //return status of Payload = PAYLOAD1_POWER_ON or PAYLOAD1_COMM_ERROR
}
