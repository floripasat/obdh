/*
 * ttc_interface_task.c
 *
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
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
 * \brief Task that deal with the TTC interface
 *
 * \author Elder Tramontin
 *
 * \version 0.2.12
 *
 * \addtogroup ttc_interface_task
 * \{
 */

#include "../interface/debug/debug.h"

#include "ttc_interface_task.h"

#define MILISECONDS_IN_A_DAY    86400000      // 1000ms * 60s * 60min * 24hr
#define TIME_TO_PROCESS_CMD     2000          /**< 2 seconds */

uint8_t send_mutex_request();
void send_shutdown();

void ttc_interface_task(void *pvParameters) {
    debug_print_event_from_module(DEBUG_INFO, "Tasks", "Initializing TTC interface task...");
    debug_new_line();

    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    uint8_t cmd_to_send = 0;
    uint8_t tx_allow = 1;
    TaskHandle_t store_task_handle;

    // get the handle of store_data_task, to stop the sampling, when in shutdown mode
    store_task_handle = xTaskGetHandle("StoreData");

    while(1) {

        xSemaphoreTake(fsp_semaphore, FSP_SEMAPHORE_WAIT_TIME);
        send_data_packet();
        xSemaphoreGive(fsp_semaphore);

        if (xQueueReceive(ttc_queue, (void *) &cmd_to_send, 100) == pdPASS) {
            if (cmd_to_send == TTC_CMD_HIBERNATION) {           // if it is a shutdown cmd
                send_shutdown();

                vTaskSuspend(store_task_handle);                // stop the store task and, by consequence, the tasks that do readings
#ifdef _DEBUG
                vTaskDelayMs(200000);                           // this delay is a shutdown simulation
#else
                vTaskDelayMs(MILISECONDS_IN_A_DAY);             // wait 24 hours
#endif
                vTaskResume(store_task_handle);
            }

            if (cmd_to_send == TTC_CMD_TX_MUTEX_REQUEST) {      // if it is a mutex request cmd
                if (send_mutex_request() == TTC_ACK) {
                    xQueueOverwrite(tx_queue, &tx_allow);
                }
                else {
                    vTaskDelayMs(2000);                         // wait until beacon release the antenna and send
                    xQueueOverwrite(tx_queue, &tx_allow);
                }
            }
        }

        // this task will be an aperiodic with 10s of max period
        if ((last_wake_time + TTC_INTERFACE_TASK_PERIOD_TICKS) < xTaskGetTickCount()) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil((TickType_t *) &last_wake_time, TTC_INTERFACE_TASK_PERIOD_TICKS);
        }
    }

    vTaskDelete( NULL );
}

void send_shutdown() {
    uint8_t ack;
    uint8_t received_data; //not used here

    xSemaphoreTake(fsp_semaphore, FSP_SEMAPHORE_WAIT_TIME);

    do {
        send_command_packet(FSP_CMD_HIBERNATION);   // send the shutdown command
        vTaskDelayMs(TIME_TO_PROCESS_CMD);          // wait 2 seconds until beacon process the received data
        ack = receive_packet(&received_data , 0);   // receive the ACK/NACK
    } while(ack != TTC_ACK);                        // repeat until receive ACK

    xSemaphoreGive(fsp_semaphore);
}

uint8_t send_mutex_request() {
    uint8_t received_data;
    uint8_t ack;

    if (xSemaphoreTake(fsp_semaphore, FSP_SEMAPHORE_WAIT_TIME) == pdPASS) {
//        send_command_packet(FSP_CMD_REQUEST_RF_MUTEX);  // send the mutex request command
//        vTaskDelayMs(TIME_TO_PROCESS_CMD);              // wait 2 seconds until beacon process the received data
//        ack = receive_packet(&received_data, 1);        // receive the ACK/NACK

        xSemaphoreGive(fsp_semaphore);
    }

    return received_data;
}

//! \} End of ttc_interface_task group
