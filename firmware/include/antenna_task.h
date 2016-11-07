/*
 * antenna_task.h
 *
 *  Created on: 04 de nov de 2016
 *      Author: elder
 */

#ifndef INCLUDE_ANTENNA_TASK_H_
#define INCLUDE_ANTENNA_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "../interface/antenna.h"

/*
 * PRIORITY =   15
 * FREQUENCY =  1Hz
 */

#define ANTENNA_TASK_PRIORITY          5
#define ANTENNA_TASK_PERIOD_MS         100
#define ANTENNA_TASK_PERIOD_TICKS      ( ANTENNA_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xAntennaTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xAntennaTask;

/**
 * \fn void prvAntennaTask( void *pvParameters )
 * That task manage the uplink data
 * \param pvParameters Not used
 */
void antennaTask( void *pvParameters );

#endif /* INCLUDE_ANTENNA_TASK_H_ */
