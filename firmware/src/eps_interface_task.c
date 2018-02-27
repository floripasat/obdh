/*
 * eps_interface_task.c
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
 * \file eps_interface_task.c
 *
 * \brief Task that deals with the EPS interface
 *
 * \author Elder Tramontin
 *
 */

#include <eps_interface_task.h>

void eps_interface_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    eps_package_t eps_package;
    uint8_t eps_status;

    eps_setup();

    while(1) {
        if (xSemaphoreTake( i2c0_semaphore, I2C_SEMAPHORE_WAIT_TIME ) == pdPASS) {  /**< try to get the mutex     */

            if(xSemaphoreTake(fsp_semaphore, FSP_SEMAPHORE_WAIT_TIME) == pdPASS) {
                eps_status = eps_read(&eps_package);
                xSemaphoreGive(fsp_semaphore);
            }

            xSemaphoreGive( i2c0_semaphore );                                       /**< release the i2c mutex    */

            if(eps_package.energy_level[0] != read_current_energy_level()) {
                xSemaphoreTake(flash_semaphore, FLASH_SEMAPHORE_WAIT_TIME);    /**< protect the flash from mutual access */
                update_energy_level(eps_package.energy_level[0]);
                xSemaphoreGive(flash_semaphore);
            }

            if(eps_status != EPS_OK) {
                eps_status = 0;
            }
            xQueueOverwrite(status_eps_queue, &eps_status);                         /**< send status (OK or NOK)  */

            if(eps_status == EPS_OK) {
                xQueueSendToBack(eps_queue, &(eps_package.eps_misc), portMAX_DELAY);  /**< send data through queue  */
            }
        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, EPS_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
