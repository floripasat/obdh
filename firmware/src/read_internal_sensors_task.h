/*
 * read_internal_sensors_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef SRC_READ_INTERNAL_SENSORS_TASK_H_
#define SRC_READ_INTERNAL_SENSORS_TASK_H_

#include "../include/msp_internal.h"
#include "../include/floripasat_def.h"
#include "FreeRTOS.h"
#include "task.h"

#define READ_INTERNAL_SENSORS_TASK_PRIORITY          5          /**< Internal sensors task priority */
#define READ_INTERNAL_SENSORS_TASK_PERIOD_MS         1000       /**< Internal sensors task period in miliseconds */
#define READ_INTERNAL_SENSORS_TASK_PERIOD_TICKS      ( READ_INTERNAL_SENSORS_TASK_PERIOD_MS / portTICK_PERIOD_MS )  /**< Internal sensors task period in ticks*/

/**
 * \var static xTaskHandle read_internal_sensors_task_handle
 * \brief variable which holds the task reference, to allow it handling
 */
static xTaskHandle read_internal_sensors_task_handle;

/**
 * \fn read_internal_sensors_task( void *pvParameters )
 * That task reads the MSP internal temperature sensor
 * \param pvParameters Not used
 * \return None
 */
void read_internal_sensors_task( void *pvParameters );

#endif /* SRC_READ_INTERNAL_SENSORS_TASK_H_ */
