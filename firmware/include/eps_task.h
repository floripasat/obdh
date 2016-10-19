/*
 * eps_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_EPS_TASK_H_
#define INCLUDE_EPS_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "../interface/eps.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define EPS_TASK_PRIORITY          5
#define EPS_TASK_PERIOD_MS         1000
#define EPS_TASK_PERIOD_TICKS      ( EPS_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xEpsTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xEpsTask;

/**
 * \fn void prvEpsTask( void *pvParameters )
 * The task witch do communication with the EPS module
 * \param pvParameters Not used
 */
void prvEpsTask( void *pvParameters );

#endif /* INCLUDE_EPS_TASK_H_ */
