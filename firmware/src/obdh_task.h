/*
 * fast_tasks.h
 *
 *  Created on: 8 de set de 2016
 *      Author: Fsat
 */

#ifndef OBDH_TASK_H_
#define OBDH_TASK_H_

#include "../interface/fsat_globals.h"
#include "../interface/obdh.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "msp430.h"

void prvOBDHTask( void *pvParameters );

#endif /* OBDH_TASK_H_ */

