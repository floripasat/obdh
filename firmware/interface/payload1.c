/*
 * payload1.c
 *
 *  Created on: 01 de jun de 2017
 *      Author: elder
 */

#include "payload1.h"

void payload1_setup(void) {

}

uint8_t payload1_read(uint8_t* data, uint32_t address, uint8_t bytes) {
    uint8_t payload1_status = PAYLOAD1_ALIVE;

    i2c_set_slave(PAYLOAD1_BASE_ADDRESS, PAYLOAD1_I2C_SLAVE_ADDRESS);

    i2c_set_mode(PAYLOAD1_BASE_ADDRESS, TRANSMIT_MODE);

    if(i2c_send_burst(PAYLOAD1_BASE_ADDRESS, (uint8_t *) &address, 4, NO_STOP) == I2C_FAIL) {
        payload1_status = PAYLOAD1_COMM_ERROR;
    }

    i2c_set_mode(PAYLOAD1_BASE_ADDRESS, RECEIVE_MODE);

    if(i2c_receive_burst(PAYLOAD1_BASE_ADDRESS, data, bytes, START_STOP) == I2C_FAIL) {
        payload1_status = PAYLOAD1_COMM_ERROR;
    }

    return payload1_status;
}

uint8_t payload1_write(uint8_t* data, uint32_t address, uint8_t bytes) {
    uint8_t payload1_status = PAYLOAD1_ALIVE;

    i2c_set_slave(PAYLOAD1_BASE_ADDRESS, PAYLOAD1_I2C_SLAVE_ADDRESS);

    i2c_set_mode(PAYLOAD1_BASE_ADDRESS, TRANSMIT_MODE);

    if(i2c_send_burst(PAYLOAD1_BASE_ADDRESS, (uint8_t *) &address, 4, NO_STOP) == I2C_FAIL) {
        payload1_status = PAYLOAD1_COMM_ERROR;
    }

    if(i2c_send_burst(PAYLOAD1_BASE_ADDRESS, data, bytes, NO_START) == I2C_FAIL) {
        payload1_status = PAYLOAD1_COMM_ERROR;
    }

    return payload1_status;
}
