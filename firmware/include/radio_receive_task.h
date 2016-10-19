/*
 * radio_receive_task.h
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_RADIO_RECEIVE_TASK_H_
#define INCLUDE_RADIO_RECEIVE_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define RADIO_RECEIVE_TASK_PRIORITY          5
#define RADIO_RECEIVE_TASK_PERIOD_MS         1000
#define RADIO_RECEIVE_TASK_PERIOD_TICKS      ( RADIO_RECEIVE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xRadioReceiveTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xRadioReceiveTask;

/**
 * \fn void prvRadioReceiveTask( void *pvParameters )
 * That task manage the uplink data
 * \param pvParameters Not used
 */
void prvRadioReceiveTask( void *pvParameters );

#endif /* INCLUDE_RADIO_RECEIVE_TASK_H_ */
