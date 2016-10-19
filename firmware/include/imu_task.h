/*
 * imu_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_IMU_TASK_H_
#define INCLUDE_IMU_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "../interface/imu.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define IMU_TASK_PRIORITY          5
#define IMU_TASK_PERIOD_MS         1000
#define IMU_TASK_PERIOD_TICKS      ( IMU_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xImuTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xImuTask;

/**
 * \fn void prvImuTask( void *pvParameters )
 * That task read the IMU data
 * \param pvParameters Not used
 */
void prvImuTask( void *pvParameters );

#endif /* INCLUDE_IMU_TASK_H_ */
