/*
 * send_down_log_task.h
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_SEND_DOWN_LOG_TASK_H_
#define INCLUDE_SEND_DOWN_LOG_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define SEND_DOWN_LOG_TASK_PRIORITY          5
#define SEND_DOWN_LOG_TASK_PERIOD_MS         1000
#define SEND_DOWN_LOG_TASK_PERIOD_TICKS      ( SEND_DOWN_LOG_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xSendDownLogTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xSendDownLogTask;

/**
 * \fn void prvSendDownLogTask( void *pvParameters )
 * That task manage the event log to be sent
 * \param pvParameters Not used
 */
void prvSendDownLogTask( void *pvParameters );

#endif /* INCLUDE_SEND_DOWN_LOG_TASK_H_ */
