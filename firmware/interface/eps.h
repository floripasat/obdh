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
#include "../util/debug.h"

void eps_read(char* data);
char* eps_data2string(char* stringBuffer, char* epsData);

#endif /* INTERFACE_EPS_H_ */
