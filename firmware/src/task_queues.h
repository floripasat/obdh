/*
 * task_queues.h
 *
 *  Created on: 14 de mar de 2017
 *      Author: elder
 */

#ifndef SRC_TASK_QUEUES_H_
#define SRC_TASK_QUEUES_H_

#include "queue.h"


/** Queues wait time in ticks. The value should be write in miliseconds and will be converted to ticks. */
#define     IMU_QUEUE_WAIT_TIME                     (100 / portTICK_PERIOD_MS)
#define     INTERNAL_SENSORS_QUEUE_WAIT_TIME        (100 / portTICK_PERIOD_MS)


volatile xQueueHandle imu_queue, internal_sensors_queue;


#endif /* SRC_TASK_QUEUES_H_ */
