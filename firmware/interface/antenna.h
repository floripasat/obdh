/*
 * antenna.h
 *
 *  Created on: 4 de nov de 2016
 *      Author: elder
 */

#ifndef INTERFACE_ANTENNA_H_
#define INTERFACE_ANTENNA_H_

#include "../driver/i2c.h"

#define NOT_DEPLOYED 0
#define DEPLOYED 1


void deploy_antenna();
uint8_t verify_deployment_status(void);

#endif /* INTERFACE_ANTENNA_H_ */
