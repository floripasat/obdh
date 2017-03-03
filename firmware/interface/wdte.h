/*
 * wdte.h
 *
 *  Created on: 23 de jan de 2017
 *      Author: elder
 */

#ifndef INTERFACE_WDTE_H_
#define INTERFACE_WDTE_H_

#include "../hal/obdh_hal.h"

void wdte_setup(void);
void wdte_reset_counter(void);

#define _DEBUG 1

#endif /* INTERFACE_WDTE_H_ */
