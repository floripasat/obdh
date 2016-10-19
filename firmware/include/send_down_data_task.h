/*
 * send_down_data_task.h
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_SEND_DOWN_DATA_TASK_H_
#define INCLUDE_SEND_DOWN_DATA_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define SEND_DOWN_DATA_TASK_PRIORITY          5
#define SEND_DOWN_DATA_TASK_PERIOD_MS         1000
#define SEND_DOWN_DATA_TASK_PERIOD_TICKS      ( SEND_DOWN_DATA_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xSendDownDataTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xSendDownDataTask;

/**
 * \fn void prvSendDownDataTask( void *pvParameters )
 * That task manage the sensors data to be sent
 * \param pvParameters Not used
 */
void prvSendDownDataTask( void *pvParameters );

#endif /* INCLUDE_SEND_DOWN_DATA_TASK_H_ */
