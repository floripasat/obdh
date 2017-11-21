/*
 * ttc_interface_task.c
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
 * \file ttc_interface_task.c
 *
 * \brief Task that deal with the TTC interface
 *
 * \author Elder Tramontin
 *
 */

#include "ttc_interface_task.h"

#define MILISECONDS_IN_A_DAY    86400000      //1000ms * 60s * 60min * 24hr
#define TIME_TO_PROCESS_CMD     2000          /**< 2 seconds */

uint8_t send_mutex_request();
void send_shutdown();

void ttc_interface_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    uint8_t cmd_to_send = 0;
    uint8_t tx_allow = 1;
    //ttc_packet_t ttc_data_packet;
    TaskHandle_t store_task_handle;

    /**< get the handle of store_data_task, to stop the sampling, when in shutdown mode */
    store_task_handle = xTaskGetHandle("StoreData");

    while(1) {

        send_data_packet();

        if(xQueueReceive(ttc_queue, (void *) &cmd_to_send, TTC_INTERFACE_TASK_PERIOD_TICKS) == pdPASS) {

            if(cmd_to_send == TTC_CMD_SHUTDOWN) {           /**< if it is a shutdown cmd */
                send_shutdown();
                vTaskSuspend(store_task_handle);                /**< stop the store task and, by consequence, the tasks that do readings */
#ifdef _DEBUG
                vTaskDelayMs(20000);                            /**< this delay is a shutdown simulation */
#else
                vTaskDelayMs(MILISECONDS_IN_A_DAY);             /**< wait 24 hours */
#endif
                vTaskResume(store_task_handle);

            }

            if(cmd_to_send == TTC_CMD_TX_MUTEX_REQUEST) {   /**< if it is a mutex request cmd */
                if(send_mutex_request() == TTC_NACK) {
                    xQueueOverwrite(tx_queue, &tx_allow);
                }
                else
                {
                    vTaskDelayMs(2000);                         /**< wait until TT&C release the antenna and send */
                    xQueueOverwrite(tx_queue, &tx_allow);
                }
            }
        }

        /**< this task will be an aperiodic with 10s of max period */
        //vTaskDelayUntil( (TickType_t *) &last_wake_time,  );
    }

    vTaskDelete( NULL );
}

void send_shutdown() {
    uint8_t received_data;

    do {
        send_command_packet(FSP_CMD_SHUTDOWN);      /**< send the shutdown command                           */
        vTaskDelayMs(TIME_TO_PROCESS_CMD);          /**< wait 2 seconds until TT&C process the received data */
        received_data = receive_packet();           /**< receive the ACK/NACK                                */

    } while(received_data != TTC_ACK);              /**< repeat until receive ACK                            */
}

uint8_t send_mutex_request() {
    uint8_t received_data;

    send_command_packet(FSP_CMD_REQUEST_RF_MUTEX);   /**< send the mutex request command */
    vTaskDelayMs(TIME_TO_PROCESS_CMD);               /**< wait 2 seconds until TT&C process the received data */
    received_data = receive_packet();                /**< receive the ACK/NACK                                */

    return received_data;
}
