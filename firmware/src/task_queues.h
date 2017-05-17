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
#define     SYSTEM_STATUS_QUEUE_WAIT_TIME           (100 / portTICK_PERIOD_MS)
#define     IMU_QUEUE_WAIT_TIME                     (100 / portTICK_PERIOD_MS)
#define     INTERNAL_SENSORS_QUEUE_WAIT_TIME        (100 / portTICK_PERIOD_MS)
#define     SOLAR_PANELS_QUEUE_WAIT_TIME            (100 / portTICK_PERIOD_MS)
#define     TRANSCEIVER_QUEUE_WAIT_TIME             (100 / portTICK_PERIOD_MS)
#define     EPS_QUEUE_WAIT_TIME                     (100 / portTICK_PERIOD_MS)
#define     PAYLOAD1_QUEUE_WAIT_TIME                (100 / portTICK_PERIOD_MS)
#define     PAYLOAD2_QUEUE_WAIT_TIME                (100 / portTICK_PERIOD_MS)


volatile xQueueHandle system_status_queue;
volatile xQueueHandle imu_queue;
volatile xQueueHandle internal_sensors_queue;
volatile xQueueHandle solar_panels_queue;
volatile xQueueHandle transceiver_queue;
volatile xQueueHandle eps_queue;
volatile xQueueHandle payload1_queue;
volatile xQueueHandle payload2_queue;


volatile xQueueHandle status_eps_queue, status_payload1_queue, status_payload2_queue, status_imu_queue, status_mem1_queue;
//TODO: mem2, antenna status_queue


#endif /* SRC_TASK_QUEUES_H_ */
