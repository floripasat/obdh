/*
 * communications_task.h
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
 * \file communications_task.h
 *
 * \brief Task that deals with the downlink and uplink communications
 *
 * \author Elder Tramontin
 *
 */

#ifndef SRC_COMMUNICATIONS_TASK_H_
#define SRC_COMMUNICATIONS_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define COMMUNICATIONS_TASK_PRIORITY          5
#define COMMUNICATIONS_TASK_PERIOD_MS         100
#define COMMUNICATIONS_TASK_PERIOD_TICKS      ( COMMUNICATIONS_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xCommunicationsTask
 * \brief variable which holds the task reference
 */
static xTaskHandle communications_task_handle;

/**
 * \fn void prvCommunicationsTask( void *pvParameters )
 * That task manage the uplink data
 * \param pvParameters Not used
 */
void communications_task( void *pvParameters );

#endif /* SRC_COMMUNICATIONS_TASK_H_ */
