/*
 * imu_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef SRC_IMU_INTERFACE_TASK_H_
#define SRC_IMU_INTERFACE_TASK_H_

#include "../interface/imu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "math.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define IMU_INTERFACE_TASK_PRIORITY          5
#define IMU_INTERFACE_TASK_PERIOD_MS         1000
#define IMU_INTERFACE_TASK_PERIOD_TICKS      ( IMU_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xImuInterfaceTask
 * \brief variable which holds the task reference
 */
static xTaskHandle imu_interface_task_handle;

/**
 * \fn void prvImuTask( void *pvParameters )
 * That task read the IMU data
 * \param pvParameters Not used
 */
void imu_interface_task( void *pvParameters );

#endif /* SRC_IMU_INTERFACE_TASK_H_ */
