/*
 * fast_tasks.h
 *
 *  Created on: 8 de set de 2016
 *      Author: Fsat
 */

#ifndef FSAT_TASKS_H_
#define FSAT_TASKS_H_


#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "adc_temp_task.h"
#include "eps_task.h"
//#include "obdh_task.h"
#include "imu_task.h"
#include "ttc_task.h"
#include "wdt_task.h"
#include "debug_task.h"

#include "msp430.h"


void vSetupHardware( void );
void vPrintEvent (char *string);

#endif

