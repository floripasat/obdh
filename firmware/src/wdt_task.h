/*
 * wdt_task.h
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
 * \file wdt_task.h
 *
 * \brief Task that deal with the watchdog timers
 *
 * \author Elder Tramontin
 *
 */

#ifndef SRC_WDT_TASK_H_
#define SRC_WDT_TASK_H_


#include "FreeRTOS.h"
#include "task.h"
#include "../driver/wdti.h"
#include "../interface/wdte.h"


#define WDT_TASK_PRIORITY          10        /**< Watchdog timer task priority */
#define WDT_TASK_PERIOD_MS         500      /**< Watchdog timer task period in milliseconds */
#define WDT_TASK_PERIOD_TICKS      ( WDT_TASK_PERIOD_MS / portTICK_PERIOD_MS ) /**< Watchdog timer task period in ticks */

/**
 * \var static xTaskHandle wdt_task_handle
 * \brief variable which holds the task reference, to allow it handling
 */
static xTaskHandle wdt_task_handle;

/**
 * \fn wdt_task( void *pvParameters )
 * That task reset the external and internal Watchdog Timers
 * \param pvParameters Not used
 * \return None
 */
void wdt_task( void *pvParameters );

#endif /* SRC_WDT_TASK_H_ */
