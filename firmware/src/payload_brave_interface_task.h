/*
 * payload1_interface_task.h
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
 * \brief Task that deals with the payload 1
 *
 * \author Akkauam Pallaoro
 *
 * \version 0.3.11
 */

#ifndef SRC_PAYLOAD_BRAVE_INTERFACE_TASK_H_
#define SRC_PAYLOAD_BRAVE_INTERFACE_TASK_H_

#define PAYLOAD_X

#include "../include/floripasat_def.h"
#include "../driver/i2c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task_queues.h"
#include "../interface/payload_brave.h"
#include "../include/msp_internal.h"
/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define PAYLOAD_BRAVE_INTERFACE_TASK_PRIORITY          2
#define PAYLOAD_BRAVE_INTERFACE_TASK_PERIOD_MS         3000
#define PAYLOAD_BRAVE_INTERFACE_TASK_PERIOD_TICKS      ( PAYLOAD_BRAVE_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle payload1_interface_task_handle
 * \brief variable which holds the task reference
 */
static xTaskHandle payload_brave_interface_task_handle;

/**
 * \fn void payload1_interface_task( void *pvParameters )
 * The task witch do communication with the payload1 module
 * \param pvParameters Not used
 */
void payload_brave_interface_task( void *pvParameters );

#endif /* SRC_PAYLOAD1_INTERFACE_TASK_H_ */
