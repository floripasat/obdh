/*
 * task_queues.h
 *
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
 *
 * This file is part of FloripaSat-OBDH.
 *
 * FloripaSat-OBDH is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FloripaSat-OBDH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /**
 * \file task_queues.h
 *
 * \brief That file contains definitions and declarations used in queues to
 * do the communication between tasks
 *
 * \author Elder Tramontin
 *
 */

#ifndef SRC_TASK_QUEUES_H_
#define SRC_TASK_QUEUES_H_

#include "queue.h"
#include "semphr.h"


/** Queues wait time in ticks. The value should be write in milliseconds and will be converted to ticks. */
//#define     SYSTEM_STATUS_QUEUE_WAIT_TIME           (100 / portTICK_PERIOD_MS)
//#define     IMU_QUEUE_WAIT_TIME                     (100 / portTICK_PERIOD_MS)
//#define     INTERNAL_SENSORS_QUEUE_WAIT_TIME        (100 / portTICK_PERIOD_MS)
//#define     SYSTEM_TIME_QUEUE_WAIT_TIME             (100 / portTICK_PERIOD_MS)
//#define     SOLAR_PANELS_QUEUE_WAIT_TIME            (100 / portTICK_PERIOD_MS)
//#define     TRANSCEIVER_QUEUE_WAIT_TIME             (100 / portTICK_PERIOD_MS)
//#define     EPS_QUEUE_WAIT_TIME                     (100 / portTICK_PERIOD_MS)
//#define     PAYLOAD1_QUEUE_WAIT_TIME                (100 / portTICK_PERIOD_MS)
//#define     PAYLOAD_BRAVE_QUEUE_WAIT_TIME                (100 / portTICK_PERIOD_MS)

#define     I2C_SEMAPHORE_WAIT_TIME                 (100 / portTICK_PERIOD_MS)
#define     SPI_SEMAPHORE_WAIT_TIME                 (100 / portTICK_PERIOD_MS)
#define     FSP_SEMAPHORE_WAIT_TIME                 (200 / portTICK_PERIOD_MS)
#define     FLASH_SEMAPHORE_WAIT_TIME               (portMAX_DELAY)

volatile xQueueHandle obdh_status_queue;
volatile xQueueHandle imu_queue;
volatile xQueueHandle obdh_misc_queue;
volatile xQueueHandle obdh_uptime_queue;
volatile xQueueHandle solar_panels_queue;
volatile xQueueHandle main_radio_queue;
volatile xQueueHandle eps_queue;
volatile xQueueHandle ttc_queue;
volatile xQueueHandle tx_queue;
volatile xQueueHandle payload_rush_queue;
volatile xQueueHandle command_to_payload_rush_queue;

volatile xQueueHandle eps_charge_queue;
volatile xQueueHandle status_eps_queue;
volatile xQueueHandle status_payload_rush_queue;
volatile xQueueHandle status_imu_queue;
volatile xQueueHandle status_mem1_queue;

#if OBDH_PAYLOAD_X_ENABLED == 1
volatile xQueueHandle payload_brave_queue;
volatile xQueueHandle status_payload_brave_queue;
volatile xQueueHandle payload_brave_uplink_queue;
volatile xQueueHandle payload_brave_downlink_queue;
#endif // OBDH_PAYLOAD_X_ENABLED

//TODO: mem2, antenna status_queue

volatile xSemaphoreHandle i2c0_semaphore;
volatile xSemaphoreHandle spi1_semaphore;
volatile xSemaphoreHandle fsp_semaphore;
volatile xSemaphoreHandle flash_semaphore;

#endif /* SRC_TASK_QUEUES_H_ */
