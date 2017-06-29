/*
 * housekeeping_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef SRC_HOUSEKEEPING_TASK_H_
#define SRC_HOUSEKEEPING_TASK_H_

#include "../include/msp_internal.h"
#include "../include/floripasat_def.h"
#include "FreeRTOS.h"
#include "task_queues.h"
#include "task.h"

#define HOUSEKEEPING_TASK_PRIORITY          5          /**< Housekeeping task priority */
#define HOUSEKEEPING_TASK_PERIOD_MS         1000       /**< Housekeeping task period in miliseconds */
#define HOUSEKEEPING_TASK_PERIOD_TICKS      ( HOUSEKEEPING_TASK_PERIOD_MS / portTICK_PERIOD_MS )  /**< Housekeeping task period in ticks*/


#define MINUTES_IN_A_DAY    1440 /**< 24 hours * 60 minutes = 1440 minutes */
/**
 * \var static xTaskHandle housekeeping_task_handle
 * \brief variable which holds the task reference, to allow it handling
 */
static xTaskHandle housekeeping_task_handle;

/**
 * \fn housekeeping_task( void *pvParameters )
 * That task check the MSP status, fault flags, reset counter and reads the
 * MSP internal temperature sensor, supply voltage and current consumption
 * \param pvParameters Not used
 * \return None
 */
void housekeeping_task( void *pvParameters );

#endif /* SRC_HOUSEKEEPING_TASK_H_ */
