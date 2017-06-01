/*
 * payload1_task.h
 *
 *  Created on: 01 de jun de 2017
 *      Author: elder
 */

#ifndef SRC_PAYLOAD1_INTERFACE_TASK_H_
#define SRC_PAYLOAD1_INTERFACE_TASK_H_

#include "../include/floripasat_def.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task_queues.h"
#include "../interface/payload1.h"
/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define PAYLOAD1_INTERFACE_TASK_PRIORITY          5
#define PAYLOAD1_INTERFACE_TASK_PERIOD_MS         1000
#define PAYLOAD1_INTERFACE_TASK_PERIOD_TICKS      ( PAYLOAD1_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle payload1_interface_task_handle
 * \brief variable which holds the task reference
 */
static xTaskHandle payload1_interface_task_handle;

/**
 * \fn void payload1_interface_task( void *pvParameters )
 * The task witch do communication with the payload1 module
 * \param pvParameters Not used
 */
void payload1_interface_task( void *pvParameters );

#endif /* SRC_PAYLOAD1_INTERFACE_TASK_H_ */
