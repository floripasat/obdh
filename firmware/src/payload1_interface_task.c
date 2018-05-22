/*
 * payload1_interface_task.c
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
 * \file payload1_interface_task.c
 *
 * \brief Task that deals with the payload 1
 *
 * \author Elder Tramontin
 *
 */

#include "payload1_interface_task.h"

void payload1_interface_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();

    uint8_t payload1_data[PAYLOAD1_DATA_LENGTH];
    uint8_t payload1_status = PAYLOAD1_POWER_OFF;
    uint8_t energy_level;
    //variables declaration


    while(1) {
        if (xSemaphoreTake( i2c0_semaphore, I2C_SEMAPHORE_WAIT_TIME ) == pdPASS) {    /**< try to get the mutex */

            energy_level = read_current_energy_level();

            switch (energy_level) {
            case ENERGY_L1_MODE:
                if(payload1_status == PAYLOAD1_POWER_OFF) {        /**< if mode is power_off, turn it on   */
                    payload1_status = PAYLOAD1_POWER_ON;
                    payload1_power_enable(1);
                }
                break;

            case ENERGY_L2_MODE:
            case ENERGY_L3_MODE:
            case ENERGY_L4_MODE:
            default:
                if(payload1_status != PAYLOAD1_POWER_OFF) {        /**< if mode is power_on, turn it off    */
                    payload1_status = PAYLOAD1_POWER_OFF;
                    payload1_power_enable(0);

                }
                break;
            }

            if(payload1_status == PAYLOAD1_POWER_ON) {

                payload1_experiment_prepare();

                // Check scratch bits
                // Check time
                // FPGA overtemperature protection
                // Check to see if there's new data to log
                /* Prepair the system for periodic power offs */

                xSemaphoreGive( i2c0_semaphore );                               //release the mutex
            }

            xQueueOverwrite(status_payload1_queue, &payload1_status);

            if(payload1_status == PAYLOAD1_POWER_ON) {
                xQueueSendToBack(payload1_queue, &payload1_data, portMAX_DELAY);
            }
        }

        if ( (last_wake_time + PAYLOAD1_INTERFACE_TASK_PERIOD_TICKS) < xTaskGetTickCount() ) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil( (TickType_t *) &last_wake_time, PAYLOAD1_INTERFACE_TASK_PERIOD_TICKS );
        }
    }

    vTaskDelete( NULL );
}
