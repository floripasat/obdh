/*
 * epsInterface_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_EPS_INTERFACE_TASK_H_
#define INCLUDE_EPS_INTERFACE_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "../interface/eps.h"
#include "../include/obdh_def.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define EPS_INTERFACE_TASK_PRIORITY          5
#define EPS_INTERFACE_TASK_PERIOD_MS         1000
#define EPS_INTERFACE_TASK_PERIOD_TICKS      ( EPS_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xEpsInterfaceTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xEpsInterfaceTask;

/**
 * \fn void epsInterfaceInterfaceTask( void *pvParameters )
 * The task witch do communication with the EPS_INTERFACE module
 * \param pvParameters Not used
 */
void epsInterfaceTask( void *pvParameters );

#endif /* INCLUDE_EPS_INTERFACE_TASK_H_ */
