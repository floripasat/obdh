/*
 * imu_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_IMU_INTERFACE_TASK_H_
#define INCLUDE_IMU_INTERFACE_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "math.h"
#include "../interface/imu.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define IMU_INTERFACE_TASK_PRIORITY          5
#define IMU_INTERFACE_TASK_PERIOD_MS         100
#define IMU_INTERFACE_TASK_PERIOD_TICKS      ( IMU_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xImuInterfaceTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xImuInterfaceTask;

/**
 * \fn void prvImuTask( void *pvParameters )
 * That task read the IMU data
 * \param pvParameters Not used
 */
void imuInterfaceTask( void *pvParameters );

#endif /* INCLUDE_IMU_INTERFACE_TASK_H_ */
