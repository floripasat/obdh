/*
 * payload_rush_interface_task.h
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
 * \file payload_rush_interface_task.h
 *
 * \brief Task that deals with the payload
 *
 * \author Andre Mattos
 *
 */

#ifndef SRC_PAYLOAD_RUSH_INTERFACE_TASK_H_
#define SRC_PAYLOAD_RUSH_INTERFACE_TASK_H_

#include "../include/floripasat_def.h"
#include "../include/msp_internal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task_queues.h"
#include "../interface/payload_rush.h"
/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define PAYLOAD_RUSH_INTERFACE_TASK_PRIORITY        5
#define PAYLOAD_RUSH_INTERFACE_TASK_PERIOD_MS       1000
#define PAYLOAD_RUSH_INTERFACE_TASK_PERIOD_TICKS    ( PAYLOAD_RUSH_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

#define EXPERIMENT_START_ITERATION        30
#define EXPERIMENT_END_ITERATION          60

/**
 * \var static xTaskHandle rush_interface_task_handle
 * \brief variable which holds the task reference
 */
static xTaskHandle payload_rush_interface_task_handle;

/**
 * \fn void payload_rush_interface_task( void *pvParameters )
 * The task witch do communication with the rush module
 * \param pvParameters Not used
 */
void payload_rush_interface_task( void *pvParameters );

/**
 * \fn void rush_delay( uint8_t time_ms )
 * This causes a delay that leaves the microcontroller free for other tasks run
 * \param time_ms time to delay in milliseconds
 */
void rush_delay_ms( uint8_t time_ms );

#endif /* SRC_PAYLOAD_RUSH_INTERFACE_TASK_H_ */
