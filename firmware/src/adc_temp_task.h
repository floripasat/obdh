/*
 * fast_tasks.h
 *
 *  Created on: 8 de set de 2016
 *      Author: Fsat
 */

#ifndef ADC_TEMP_TASK_H_
#define ADC_TEMP_TASK_H_

#include "../driver/adc.h"
#include "../interface/fsat_globals.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "msp430.h"


void prvReadTempTask( void *pvParameters );


// Calibration constant for ADC 1.5-V Reference, Temp. Sensor 30°C
#define CALADC12_15V_30C        (*((unsigned int *)0x1A1A))
// Calibration constant for ADC 1.5-V Reference, Temp. Sensor 85°C
#define CALADC12_15V_85C        (*((unsigned int *)0x1A1C))


#endif /* ADC_TEMP_TASK_H_ */

