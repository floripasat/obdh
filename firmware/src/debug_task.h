/*
 * debug_task.h
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
 * \file debug_task.h
 *
 * \brief Task that deals with the debug interface
 *
 * \author Elder Tramontin
 *
 */

#ifndef INCLUDE_DEBUG_TASK_XH_
#define INCLUDE_DEBUG_TASK_XH_

#include "FreeRTOS.h"
#include "task.h"
#include "../include/msp_internal.h"
#include "../include/floripasat_def.h"
#include "../driver/uart.h"
#include "../interface/ttc.h"
#include "../util/link.h"
#include "store_data_task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define DEBUG_TASK_PRIORITY          5
#define DEBUG_TASK_PERIOD_MS         500
#define DEBUG_TASK_PERIOD_TICKS      ( DEBUG_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xDebugTask;
 * \brief variable which holds the task reference
 */
static xTaskHandle debug_task_handle;

/**
 * \fn void prvDebugTask( void *pvParameters )
 * The task witch do the Debug module main function
 * \param pvParameters Not used
 */
void debug_task( void *pvParameters );

#endif /* INCLUDE_DEBUG_TASK_XH_ */
