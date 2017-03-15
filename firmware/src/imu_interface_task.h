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
#include "queue.h"
#include "task_queues.h"
#include "math.h"


#define IMU_INTERFACE_TASK_PRIORITY          5          /**< IMU task priority */
#define IMU_INTERFACE_TASK_PERIOD_MS         1000       /**< IMU task period in miliseconds */
#define IMU_INTERFACE_TASK_PERIOD_TICKS      ( IMU_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )  /**< IMU task period in ticks */

/**
 * \var static xTaskHandle imu_interface_task_handle
 * \brief variable which holds the task reference, to allow it handling
 */
static xTaskHandle imu_interface_task_handle;

/**
 * \fn imu_interface_task( void *pvParameters )
 * That task read the IMU data
 * \param pvParameters Not used
 * \return None
 */
void imu_interface_task( void *pvParameters );

#endif /* SRC_IMU_INTERFACE_TASK_H_ */
