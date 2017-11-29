/*
 * solar_panel_interface_task.h
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
 * \file solar_panel_interface_task.h
 *
 * \brief Task that deals with the solar panels modules
 *
 * \author Elder Tramontin
 *
 */

#ifndef SRC_SOLAR_PANELS_INTERFACE_TASK_H_
#define SRC_SOLAR_PANELS_INTERFACE_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "../src/task_queues.h"
#include "../interface/solar_panel.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  0.2Hz
 */

#define SOLAR_PANELS_INTERFACE_TASK_PRIORITY          3
#define SOLAR_PANELS_INTERFACE_TASK_PERIOD_MS         5000
#define SOLAR_PANELS_INTERFACE_TASK_PERIOD_TICKS      ( SOLAR_PANELS_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xSolarPanelsInterfaceTask
 * \brief variable which holds the task reference
 */
static xTaskHandle solar_panels_interface_task_handle;

/**
 * \fn void prvSolarPanelsTask( void *pvParameters )
 * That task manage the solar panels sensors
 * \param pvParameters Not used
 */
void solar_panels_interface_task( void *pvParameters );

#endif /* SRC_SOLAR_PANELS_INTERFACE_TASK_H_ */
