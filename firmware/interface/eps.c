/*
 * eps.c
 *
 *  Created on: 11 de mai de 2016
 *      Author: mario
 */

#include <eps.h>

void eps_setup(void) {

}


uint8_t eps_read(eps_package_t *package) {
//     Clear frame memory space
    uint8_t *data = (uint8_t *)package;
    uint8_t eps_status = EPS_ALIVE;

    i2c_set_mode(EPS_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send(EPS_BASE_ADDRESS, EPS_REQUEST_DATA_CMD, NO_STOP);

    i2c_set_mode(EPS_BASE_ADDRESS, RECEIVE_MODE);
    if(i2c_receive(EPS_BASE_ADDRESS, data, NO_STOP) == I2C_FAIL) {
        eps_status = EPS_ERROR;
    }

    if(i2c_receive_burst(EPS_BASE_ADDRESS, (data + 1), EPS_PACKAGE_LENGTH - 2) == I2C_FAIL) {
        eps_status = EPS_ERROR;
    }

    if(i2c_receive(EPS_BASE_ADDRESS, (data + EPS_PACKAGE_LENGTH - 1), NO_START) == I2C_FAIL) {
        eps_status = EPS_ERROR;
    }
    return eps_status;
}


