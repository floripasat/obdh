/*
 * eps.c
 *
 *  Created on: 11 de mai de 2016
 *      Author: mario
 */

#include <eps.h>

void eps_setup(void) {
//    i2c_setup(0);
}


void eps_read(eps_package_t *package) {
//     Clear frame memory space
    uint8_t *data = (uint8_t *)package;

    i2c_send(EPS_BASE_ADDRESS, EPS_REQUEST_DATA_CMD, NO_STOP);

    data[0] = i2c_receive(EPS_BASE_ADDRESS, NO_STOP);

    i2c_receive_burst(EPS_BASE_ADDRESS, data + 1, EPS_PACKAGE_LENGTH - 2);

    data[EPS_PACKAGE_LENGTH] = i2c_receive(EPS_BASE_ADDRESS, NO_START);
}


