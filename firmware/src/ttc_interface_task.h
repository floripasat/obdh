/*
 * ttc_interface_task.h
 *
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina
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
 * \brief Task that deal with the TTC interface
 *
 * \author Elder Tramontin
 *
 * \version 0.2.12
 *
 * \defgroup ttc_interface_task TTC Task
 * \ingroup tasks
 * \{
 */

#ifndef SRC_TTC_INTERFACE_TASK_H_
#define SRC_TTC_INTERFACE_TASK_H_

#include "../include/floripasat_def.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task_queues.h"
#include "../interface/ttc.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  0.2Hz
 */
#define TTC_INTERFACE_TASK_PRIORITY         5
#define TTC_INTERFACE_TASK_PERIOD_MS        5000
#define TTC_INTERFACE_TASK_PERIOD_TICKS     ( TTC_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )
//#define TTC_QUEUE_WAIT_TIME                 ( 10 / portTICK_PERIOD_MS )

/**
 * \brief variable which holds the task reference
 */
static xTaskHandle ttc_interface_task_handle;

/**
 * The task witch do communication with the TTC_INTERFACE module
 * \param pvParameters Not used
 */
void ttc_interface_task(void *pvParameters);

#endif /* SRC_TTC_INTERFACE_TASK_H_ */

//! \} End of ttc_interface_task group
