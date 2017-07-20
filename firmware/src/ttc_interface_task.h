/*
 * ttc_interface_task.h
 *
 *  Created on: 18 de jan de 2017
 *      Author: elder
 */

#ifndef SRC_TTC_INTERFACE_TASK_H_
#define SRC_TTC_INTERFACE_TASK_H_

#include "../include/floripasat_def.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task_queues.h"
#include "../interface/ttc.h"
#include "../util/crc.h"
/*
 * PRIORITY =   5
 * FREQUENCY =  0.2Hz
 */

#define TTC_INTERFACE_TASK_PRIORITY         5
#define TTC_INTERFACE_TASK_PERIOD_MS        5000
#define TTC_INTERFACE_TASK_PERIOD_TICKS     ( TTC_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )
#define TTC_QUEUE_WAIT_TIME                 ( 10 / portTICK_PERIOD_MS )
/**
 * \var static xTaskHandle xTtcInterfaceTask
 * \brief variable which holds the task reference
 */
static xTaskHandle ttc_interface_task_handle;

/**
 * \fn void ttc_interfaceTask( void *pvParameters )
 * The task witch do communication with the TTC_INTERFACE module
 * \param pvParameters Not used
 */
void ttc_interface_task( void *pvParameters );

#endif /* SRC_TTC_INTERFACE_TASK_H_ */
