/*
 * antenna.h
 *
 *  Created on: 4 de nov de 2016
 *      Author: elder
 */

#ifndef INTERFACE_ANTENNA_H_
#define INTERFACE_ANTENNA_H_

#include "../driver/i2c.h"

#define     ANTENNA_NOT_DEPLOYED    0
#define     ANTENNA_DEPLOYED        1


void antenna_deployment_routine(void);
uint8_t verify_deployment_status(void);

#endif /* INTERFACE_ANTENNA_H_ */
