/*
 * attitude_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_ATTITUDE_TASK_H_
#define INCLUDE_ATTITUDE_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define ATTITUDE_TASK_PRIORITY          5
#define ATTITUDE_TASK_PERIOD_MS         1000
#define ATTITUDE_TASK_PERIOD_TICKS      ( ATTITUDE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xAttitudeTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xAttitudeTask;

/**
 * \fn void prvAttitudeTask( void *pvParameters )
 * The task witch do the Attitude module main function
 * \param pvParameters Not used
 */
void prvAttitudeTask( void *pvParameters );

#endif /* INCLUDE_ATTITUDE_TASK_H_ */
