/*
 * eps.h
 *
 *  Created on: 11 de mai de 2016
 *      Author: mario
 */

#ifndef INTERFACE_EPS_H_
#define INTERFACE_EPS_H_

#include "../driver/i2c.h"
#include "../include/floripasat_def.h"

#define EPS_REQUEST_DATA_CMD   0x0F      //an arbitrary value was choosen


/* EPS FRAME */
typedef struct {
    uint8_t msp430[26];
    uint8_t battery_monitor[21];
    uint8_t ads1248[21];
    uint8_t task_scheduler[1];
} eps_package_t;

#define EPS_PACKAGE_LENGTH  sizeof(eps_package_t)


void eps_setup(void);
void eps_read(eps_package_t *package);

#endif /* INTERFACE_EPS_H_ */
