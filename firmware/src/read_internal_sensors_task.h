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
//#include "../driver/adc.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define READ_INTERNAL_SENSORS_TASK_PRIORITY          5
#define READ_INTERNAL_SENSORS_TASK_PERIOD_MS         1000
#define READ_INTERNAL_SENSORS_TASK_PERIOD_TICKS      ( READ_INTERNAL_SENSORS_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xReadInternalSensorsTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xReadInternalSensorsTask;

/**
 * \fn void prvReadInternalSensorsTask( void *pvParameters )
 * That task reads the MSP internal temperature sensor
 * \param pvParameters Not used
 */
void readInternalSensorsTask( void *pvParameters );

#endif /* SRC_READ_INTERNAL_SENSORS_TASK_H_ */
