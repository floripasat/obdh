/*
 * eps_interface_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef SRC_EPS_INTERFACE_TASK_H_
#define SRC_EPS_INTERFACE_TASK_H_

#include "../include/floripasat_def.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task_queues.h"
#include "../interface/eps.h"

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
static xTaskHandle eps_interface_task_handle;

/**
 * \fn void epsInterfaceInterfaceTask( void *pvParameters )
 * The task witch do communication with the EPS_INTERFACE module
 * \param pvParameters Not used
 */
void eps_interface_task( void *pvParameters );

#endif /* SRC_EPS_INTERFACE_TASK_H_ */
