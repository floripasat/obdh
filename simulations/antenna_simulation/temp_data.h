/*
 * temp_data.c
 *
 *  Created on: 25 de jul de 2017
 *      Author: André
 */

#ifndef TEMP_DATA_H_
#define TEMP_DATA_H_

#include <msp430.h>
#include <stdint.h>
#include "misc_def.h"

void temperature_setup(void);

uint16_t temperature_read(void);

#endif /* TEMP_DATA_H_ */
