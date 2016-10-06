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

#include "hal/uart.h"
#include "hal/adc.h"

//tasks files
#include "interface/adc_temp_task.h"
#include "interface/eps_task.h"
#include "interface/imu_task.h"
#include "interface/ttc_task.h"
#include "interface/uart_task.h"

#include "msp430.h"


void vSetupHardware( void );


#endif

