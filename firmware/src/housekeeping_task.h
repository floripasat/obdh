/*
 * housekeeping_task.h
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
 * \file housekeeping_task.h
 *
 * \brief Task that do the housekeeping: update time counter,
 * read the supply voltage and current and manage the operation modes
 *
 * \author Elder Tramontin
 *
 */

#ifndef SRC_HOUSEKEEPING_TASK_H_
#define SRC_HOUSEKEEPING_TASK_H_

#include "../include/msp_internal.h"
#include "../include/floripasat_def.h"
#include "FreeRTOS.h"
#include "task_queues.h"
#include "task.h"

#define HOUSEKEEPING_TASK_PRIORITY          5          /**< Housekeeping task priority */
#define HOUSEKEEPING_TASK_PERIOD_MS         1000       /**< Housekeeping task period in miliseconds */
#define HOUSEKEEPING_TASK_PERIOD_TICKS      ( HOUSEKEEPING_TASK_PERIOD_MS / portTICK_PERIOD_MS )  /**< Housekeeping task period in ticks*/


#define MINUTES_IN_A_DAY    1440 /**< 24 hours * 60 minutes = 1440 minutes */
/**
 * \var static xTaskHandle housekeeping_task_handle
 * \brief variable which holds the task reference, to allow it handling
 */
static xTaskHandle housekeeping_task_handle;

/**
 * \fn housekeeping_task( void *pvParameters )
 * That task check the MSP status, fault flags, reset counter and reads the
 * MSP internal temperature sensor, supply voltage and current consumption
 * \param pvParameters Not used
 * \return None
 */
void housekeeping_task( void *pvParameters );

#endif /* SRC_HOUSEKEEPING_TASK_H_ */
