/*
 * radio_transmit_task.h
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_RADIO_TRANSMIT_TASK_H_
#define INCLUDE_RADIO_TRANSMIT_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define RADIO_TRANSMIT_TASK_PRIORITY          5
#define RADIO_TRANSMIT_TASK_PERIOD_MS         1000
#define RADIO_TRANSMIT_TASK_PERIOD_TICKS      ( RADIO_TRANSMIT_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xRadioTransmitTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xRadioTransmitTask;

/**
 * \fn void prvRadioTransmitTask( void *pvParameters )
 * That task manage the downlink data
 * \param pvParameters Not used
 */
void prvRadioTransmitTask( void *pvParameters );

#endif /* INCLUDE_RADIO_TRANSMIT_TASK_H_ */
