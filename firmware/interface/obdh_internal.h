/*
 * obdh.h
 *
 *  Created on: 30 de mai de 2016
 *      Author: mario
 */

// Interface with internal components of the OBDH SoC


#ifndef INCLUDE_OBDH_H_
#define INCLUDE_OBDH_H_

#include "../include/obdh_def.h"
#include "../driver/obdh_hal.h"
#include "../driver/adc.h"
#include "../util/debug.h"

#include "stdint.h"

unsigned int obdhTemperatureBuffer;
volatile float temperatureDegC;
volatile float temperatureDegF;

char* obdh_data2string(char* stringBuffer, char* obdhData);

//void obdh_read(char* obdhData);
float obdh_temp_convert(uint16_t usTempRaw);
uint16_t obdh_temp_read(void);
void obdh_setup(void);

float TEMP_COEFICIENT;

#endif /* INCLUDE_OBDH_H_ */
