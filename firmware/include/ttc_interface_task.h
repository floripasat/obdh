/*
 * ttc_interface_task.h
 *
 *  Created on: 18 de jan de 2017
 *      Author: elder
 */

#ifndef INCLUDE_TTC_INTERFACE_TASK_H_
#define INCLUDE_TTC_INTERFACE_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
//#include "../interface/ttc.h"
#include "../include/obdh_def.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define TTC_INTERFACE_TASK_PRIORITY          5
#define TTC_INTERFACE_TASK_PERIOD_MS         1000
#define TTC_INTERFACE_TASK_PERIOD_TICKS      ( TTC_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xTtcInterfaceTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xTtcInterfaceTask;

/**
 * \fn void ttc_interfaceTask( void *pvParameters )
 * The task witch do communication with the TTC_INTERFACE module
 * \param pvParameters Not used
 */
void ttcInterfaceTask( void *pvParameters );

#endif /* INCLUDE_TTC_INTERFACE_TASK_H_ */
