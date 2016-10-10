/*
 * fast_tasks.h
 *
 *  Created on: 8 de set de 2016
 *      Author: Fsat
 */

#ifndef TTC_TASK_
#define TTC_TASK_

#include "../interface/fsat_globals.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "msp430.h"


void prvTTCTask( void *pvParameters );


#endif /* TTC_TASK_ */

