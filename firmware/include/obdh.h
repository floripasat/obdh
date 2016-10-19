/*
 * obdh.h
 *
 *  Created on: 8 de set de 2016
 *      Author: Fsat
 */

#ifndef INCLUDE_OBDH_H_
#define INCLUDE_OBDH_H_


#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "../driver/obdh_hal.h"

#include "../include/imu_task.h"
//#include "../include/ttc_task.h"
//#include "../include/wdt_task.h"
//#include "../include/debug_task.h"
//#include "../driver/obdh_hal.h"


void vDeployAntenna();
void prvCreateTasks();
void vSetupHardware( void );
//void vPrintEvent (char *string);


#endif
