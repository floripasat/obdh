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
    uint8_t payload1_status;

    uint8_t test_data[PAYLOAD1_DATA_LENGTH] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; //just for tests purpose

    while(1) {
        //TODO: TASK ROUTINE

        if (xSemaphoreTake( i2c0_semaphore, I2C_SEMAPHORE_WAIT_TIME ) == pdPASS) {    //try to get the mutex

//            payload1_status = payload1_write(test_data, 0x00000005, 10);  //write 10 bytes starting on the address 5

            payload1_status = payload1_read(payload1_data, 0x00000005, 2);  //read the temperature register (2 bytes)

            xSemaphoreGive( i2c0_semaphore );                               //release the mutex

            xQueueOverwrite(status_payload1_queue, &payload1_status);

            if(payload1_status == PAYLOAD1_ALIVE) {
                xQueueSendToBack(payload1_queue, &payload1_data, portMAX_DELAY);
            }
        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, PAYLOAD1_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
