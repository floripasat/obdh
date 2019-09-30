/*
 * payload_brave_interface_task.c
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
 * \file PAYLOAD_BRAVE_interface_task.c
 *
 * \brief Task that deals with the payload x
 *
 * \author Akkauam Pallaoro
 *
 */

#include "payload_brave_interface_task.h"

#if OBDH_PAYLOADX_ENABLED == 1
void payload_brave_interface_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();

    payload_brave_uplink_t write_pkt;
    payload_brave_downlink_t read_pkt;
    uint8_t payload_brave_status = PAYLOAD_BRAVE_POWER_OFF;
    uint8_t energy_level;

    payload_brave_setup();

    while(1) {
        //TODO: TASK ROUTINE
        energy_level = read_current_energy_level();

        switch (energy_level) {
        case ENERGY_L1_MODE:
            //if(payload_brave_status == PAYLOAD_BRAVE_POWER_OFF) {        /**< if mode is power_off, turn it on   */
            payload_brave_status = PAYLOAD_BRAVE_POWER_ON;
                /**< send power on command              */
            //}
            break;

        case ENERGY_L2_MODE:    break;
        case ENERGY_L3_MODE:
        case ENERGY_L4_MODE:
        default:
            //if(payload_brave_status != PAYLOAD_BRAVE_POWER_OFF) {        /**< if mode is power_on, turn it off    */
            payload_brave_status = PAYLOAD_BRAVE_POWER_OFF;
                /**< send power off command              */

            //}
                break;
        }

        if(payload_brave_status == PAYLOAD_BRAVE_POWER_ON) {
            payload_brave_power_on();
            if(xSemaphoreTake(fsp_semaphore, FSP_SEMAPHORE_WAIT_TIME) == pdPASS) {    /**< try to get the mutex */
                if (xSemaphoreTake( i2c0_semaphore, I2C_SEMAPHORE_WAIT_TIME ) == pdPASS) {    /**< try to get the mutex */
                    if(xQueueReceive(payload_brave_uplink_queue, &write_pkt, 0) == pdPASS) {

                        payload_brave_status = payload_brave_write(&write_pkt);

                        if(payload_brave_status != PAYLOAD_BRAVE_OK) {
                            payload_brave_status = 0;
                        }

                        xQueueOverwrite(status_payload_brave_queue, &payload_brave_status);
                    }



                    xSemaphoreGive( i2c0_semaphore );                               //release the mutex

                }

                if (xSemaphoreTake( i2c0_semaphore, I2C_SEMAPHORE_WAIT_TIME ) == pdPASS) {    /**< try to get the mutex */

                    payload_brave_status = payload_brave_read(&read_pkt);

                    xSemaphoreGive( i2c0_semaphore );                               //release the mutex

                    if(payload_brave_status != PAYLOAD_BRAVE_OK) {
                        payload_brave_status = 0;
                    }

//                    xQueueOverwrite(status_payload_brave_queue, &payload_brave_brave_status);                         /**< send status (OK or NOK)  */

                    if(payload_brave_status == PAYLOAD_BRAVE_OK) {
                        xQueueSendToBack(payload_brave_downlink_queue, &read_pkt, portMAX_DELAY);          /**< send data through queue  */
                        xQueueSendToBack(payload_brave_queue, &read_pkt, portMAX_DELAY);          /**< send data through queue  */
                    }

                }

                xSemaphoreGive(fsp_semaphore);              //release the mutex
            }
        }else
        {
            payload_brave_power_down();
        }

        if ( (last_wake_time + PAYLOAD_BRAVE_INTERFACE_TASK_PERIOD_TICKS) < xTaskGetTickCount() ) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil( (TickType_t *) &last_wake_time, PAYLOAD_BRAVE_INTERFACE_TASK_PERIOD_TICKS );
        }
    }

    vTaskDelete( NULL );
}
#endif // OBDH_PAYLOADX_ENABLED
