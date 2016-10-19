/*
 * ttc_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_TTC_TASK_H_
#define INCLUDE_TTC_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
//#include "../interface/ttc.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define TTC_TASK_PRIORITY          5
#define TTC_TASK_PERIOD_MS         1000
#define TTC_TASK_PERIOD_TICKS      ( TTC_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xTtcTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xTtcTask;

/**
 * \fn void prvTtcTask( void *pvParameters )
 * That task reads the TTC data
 * \param pvParameters Not used
 */
void prvTtcTask( void *pvParameters );

#endif /* INCLUDE_TTC_TASK_H_ */
