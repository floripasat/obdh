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

void ttc_interface_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    uint8_t ttc_cmd_to_send = 0;
    uint8_t tx_allow = 1;
    //ttc_packet_t ttc_data_packet;
    TaskHandle_t store_task_handle;

    /* FloripaSat communication protocol */
    uint8_t ttc_pkt[FSP_PKT_MAX_LENGTH];

    /**< get the handle of store_data_task, to stop the sampling, when in shutdown mode */
    store_task_handle = xTaskGetHandle("StoreData");

    while(1) {
        //ttc_data_packet.data = ttc_copy_data();

        //ttc_data_packet.start_of_frame  = START_OF_FRAME;                   /**< initial byte of the frame: '{' */
        //ttc_data_packet.crc_result      = crc8(CRC_SEED,
        //                                       CRC_POLYNOMIAL,
        //                                       (uint8_t*) &(ttc_data_packet.data),
        //                                       sizeof(beacon_packet_t) );   /**< calculate the 8-bits CRC value of beacon data */
        fsp_obdh_ttc_packet(&ttc_pkt);
        ttc_send_data(&ttc_pkt);

        if(xQueueReceive(ttc_queue, (void *) &ttc_cmd_to_send, TTC_QUEUE_WAIT_TIME) == pdPASS) {

            if(ttc_cmd_to_send == TTC_CMD_SHUTDOWN) {           /**< if it is a shutdown cmd */
                ttc_send_shutdown();

                vTaskSuspend(store_task_handle);                /**< stop the store task and, by consequence, the tasks that do readings */
#ifdef _DEBUG
                vTaskDelayMs(20000);                            /**< this delay is a shutdown simulation */
#else
                vTaskDelayMs(MILISECONDS_IN_A_DAY);             /**< wait 24 hours */
#endif
                vTaskResume(store_task_handle);

            }

            if(ttc_cmd_to_send == TTC_CMD_TX_MUTEX_REQUEST) {   /**< if it is a mutex request cmd */
                while(ttc_send_mutex_request() == TTC_TX_BUSY) {
                    vTaskDelayMs(200); /**< wait until TT&C release the mutex */
                }

                xQueueOverwrite(tx_queue, &tx_allow);
                //TODO: transmit_routine
            }

            if(ttc_cmd_to_send == TTC_CMD_TX_MUTEX_RELEASE) {   /**< if it is a mutex release cmd */
                ttc_tx_mutex_release();
            }
        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, TTC_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
