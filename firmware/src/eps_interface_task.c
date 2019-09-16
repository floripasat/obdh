/*
 * eps_interface_task.c
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
 * \brief Task that deals with the EPS interface
 *
 * \author Elder Tramontin
 *
 * \version 0.2.13
 *
 * \addtogroup eps_interface
 * \{
 */

#include "../interface/debug/debug.h"

#include <eps_interface_task.h>

void eps_interface_task(void *pvParameters) {
    debug_print_event_from_module(DEBUG_INFO, "Tasks", "Initializing EPS interface task...");
    debug_new_line();

    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    eps_package_t eps_package;
    uint8_t eps_status;
    uint8_t cmd_to_send = 0;
    uint8_t send_attempts = 0;

    eps_setup();

    while(1) {
        if (xSemaphoreTake(i2c0_semaphore, I2C_SEMAPHORE_WAIT_TIME) == pdPASS) {        // try to get the mutex
            if(xSemaphoreTake(fsp_semaphore, FSP_SEMAPHORE_WAIT_TIME) == pdPASS) {
                eps_status = eps_read(&eps_package);

                if (xQueueReceive(eps_charge_queue, (void *) &cmd_to_send, 0) == pdPASS) {
                    if (cmd_to_send == EPS_CHARGE_RESET_CMD) {                          // if it is a charge reset cmd
                        for(send_attempts = 0; send_attempts <= 3; send_attempts++) {
                            send_command_charge_reset();
                        }
                    }
                }
                xSemaphoreGive(fsp_semaphore);
            }

            xSemaphoreGive(i2c0_semaphore);                                             // release the i2c mutex

            if (eps_package.energy_level[0] != read_current_energy_level()) {
                xSemaphoreTake(flash_semaphore, FLASH_SEMAPHORE_WAIT_TIME);             // protect the flash from mutual access
                update_energy_level(eps_package.energy_level[0]);
                xSemaphoreGive(flash_semaphore);
            }

            if (eps_status != EPS_OK) {
                debug_print_event_from_module(DEBUG_WARNING, "EPS Interface Task", "EPS module not responding!");
                debug_new_line();

                eps_status = 0;
            }

            xQueueOverwrite(status_eps_queue, &eps_status);                             // send status (OK or NOK)

            if (eps_status == EPS_OK) {
                xQueueSendToBack(eps_queue, &(eps_package.eps_misc), portMAX_DELAY);    // send data through queue
            }
        }

        if ((last_wake_time + EPS_INTERFACE_TASK_PERIOD_TICKS) < xTaskGetTickCount()) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil((TickType_t *) &last_wake_time, EPS_INTERFACE_TASK_PERIOD_TICKS);
        }
    }

    vTaskDelete(NULL);
}

//! \} End of eps_interface group
