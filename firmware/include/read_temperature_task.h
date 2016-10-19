/*
 * read_temperature_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_READ_TEMPERATURE_TASK_H_
#define INCLUDE_READ_TEMPERATURE_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "../driver/adc.h"
#include "../include/obdh_def.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define READ_TEMPERATURE_TASK_PRIORITY          5
#define READ_TEMPERATURE_TASK_PERIOD_MS         1000
#define READ_TEMPERATURE_TASK_PERIOD_TICKS      ( READ_TEMPERATURE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xReadTemperatureTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xReadTemperatureTask;

/**
 * \fn void prvReadTemperatureTask( void *pvParameters )
 * That task reads the MSP internal temperature sensor
 * \param pvParameters Not used
 */
void prvReadTemperatureTask( void *pvParameters );

#endif /* INCLUDE_READ_TEMPERATURE_TASK_H_ */
