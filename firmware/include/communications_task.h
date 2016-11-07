/*
 * communications_task.h
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_COMMUNICATIONS_TASK_H_
#define INCLUDE_COMMUNICATIONS_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define COMMUNICATIONS_TASK_PRIORITY          5
#define COMMUNICATIONS_TASK_PERIOD_MS         100
#define COMMUNICATIONS_TASK_PERIOD_TICKS      ( COMMUNICATIONS_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xCommunicationsTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xCommunicationsTask;

/**
 * \fn void prvCommunicationsTask( void *pvParameters )
 * That task manage the uplink data
 * \param pvParameters Not used
 */
void communicationsTask( void *pvParameters );

#endif /* INCLUDE_COMMUNICATIONS_TASK_H_ */
