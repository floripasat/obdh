/*
 * fast_tasks.h
 *
 *  Created on: 8 de set de 2016
 *      Author: Fsat
 */

#ifndef IMU_TASK_H_
#define IMU_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "globals.h"

#include "msp430.h"

void prvIMUTask( void *pvParameters );


#endif /* IMU_TASK_H_ */

