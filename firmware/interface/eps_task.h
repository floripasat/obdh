/*
 * fast_tasks.h
 *
 *  Created on: 8 de set de 2016
 *      Author: Fsat
 */

#ifndef EPS_TASK_H_
#define EPS_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "globals.h"

#include "msp430.h"

void prvEPSTask( void *pvParameters );

#endif /* EPS_TASK_H_ */

