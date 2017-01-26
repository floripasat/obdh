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

#define RSENSE_VALUE 47000.0 //ohms
#define CURRENT_GAIN 0.005 // A/V
#define RL_VALUE 0.05 // A/V


unsigned int obdhTemperatureBuffer;
volatile float temperatureDegC;
volatile float temperatureDegF;

char* obdh_data2string(char* stringBuffer, char* obdhData);

//void obdh_read(char* obdhData);
float obdh_temperature_convert(uint16_t usTempRaw);
uint16_t obdh_temperature_read(void);
uint16_t obdh_current_read(void);
float obdh_current_convert(uint16_t curr_raw);
uint16_t obdh_voltage_read(void);
float obdh_voltage_convert(uint16_t volt_raw);
void obdh_setup(void);

#endif /* INCLUDE_OBDH_H_ */
