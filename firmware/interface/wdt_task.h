/*
 * fast_tasks.h
 *
 *  Created on: 6 de out de 2016
 *      Author: Fsat
 */

#ifndef WDT_TASK_
#define WDT_TASK_

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "globals.h"

#include "msp430.h"


void prvWdtTask( void *pvParameters );


#endif /* WDT_TASK_ */

