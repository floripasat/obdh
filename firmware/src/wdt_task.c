/*
 * wdt_task.c
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
 * \file wdt_task.c
 *
 * \brief Task that deal with the watchdog timers
 *
 * \author Elder Tramontin
 *
 */

#include "wdt_task.h"

void wdt_task( void *pvParameters )
{
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();

    while(1) {
        wdti_reset_counter();                       /**< Reset internal watchdog timer */
        wdte_reset_counter();                       /**< Reset external watchdog timer */
    #ifdef _DEBUG
        BIT_TOGGLE(LED_SYSTEM_OUT, LED_SYSTEM_PIN); /**< Blink a led just for debug purpose */
    #endif

        if ( (last_wake_time + WDT_TASK_PERIOD_TICKS) < xTaskGetTickCount() ) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil( (TickType_t *) &last_wake_time, WDT_TASK_PERIOD_TICKS );
        }
    }

    vTaskDelete( NULL );
}

