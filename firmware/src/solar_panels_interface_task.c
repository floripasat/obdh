/*
 * solar_panel_interface_task.c
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
 * \file solar_panel_interface_task.c
 *
 * \brief Task that deals with the solar panels modules
 *
 * \author Elder Tramontin
 *
 */

#include "solar_panels_interface_task.h"

void solar_panels_interface_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    int16_t solar_panel_temperature_x, solar_panel_temperature_y, solar_panel_temperature_z;

    if (xSemaphoreTake( spi1_semaphore, portMAX_DELAY ) == pdPASS) {
        solar_panel_setup();
        xSemaphoreGive( spi1_semaphore );
    }
    while(1) {
        if (xSemaphoreTake( spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME ) == pdPASS) {
            solar_panel_temperature_x = solar_panel_read_temperature(SOLAR_PANEL_X);
            solar_panel_temperature_y = solar_panel_read_temperature(SOLAR_PANEL_Y);
            solar_panel_temperature_z = solar_panel_read_temperature(SOLAR_PANEL_Z);
            xSemaphoreGive( spi1_semaphore );
        }

        if ( (last_wake_time + SOLAR_PANELS_INTERFACE_TASK_PERIOD_TICKS) < xTaskGetTickCount() ) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil( (TickType_t *) &last_wake_time, SOLAR_PANELS_INTERFACE_TASK_PERIOD_TICKS );
        }
    }

    vTaskDelete( NULL );
}
