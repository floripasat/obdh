/*
 * write_log_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_WRITE_LOG_TASK_H_
#define INCLUDE_WRITE_LOG_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define WRITE_LOG_TASK_PRIORITY          5
#define WRITE_LOG_TASK_PERIOD_MS         1000
#define WRITE_LOG_TASK_PERIOD_TICKS      ( WRITE_LOG_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xWriteLogTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xWriteLogTask;

/**
 * \fn void prvWriteLogTask( void *pvParameters )
 * The task witch do the WriteLog module main function
 * \param pvParameters Not used
 */
void prvWriteLogTask( void *pvParameters );

#endif /* INCLUDE_WRITE_LOG_TASK_H_ */
