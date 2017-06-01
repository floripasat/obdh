/*
 * payload1_interface_task.c
 *
 *  Created on: 01 de jun de 2017
 *      Author: elder
 */

#include <payload1_interface_task.h>

void payload1_interface_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();

    uint8_t payload1_data[PAYLOAD1_DATA_LENGTH];
    uint8_t payload1_status;

    uint8_t test_data[PAYLOAD1_DATA_LENGTH] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; //just for tests purpose

    while(1) {
        //TODO: TASK ROUTINE

        if (xSemaphoreTake( i2c0_semaphore, I2C_SEMAPHORE_WAIT_TIME ) == pdPASS) {    //try to get the mutex

            payload1_status = payload1_write(test_data, 0x00000005, 10);

            payload1_status = payload1_read(payload1_data, 0x00000005, 10);

            xQueueOverwrite(status_payload1_queue, &payload1_status);

            if(payload1_status == PAYLOAD1_ALIVE) {
                xQueueSendToBack(payload1_queue, &payload1_data, portMAX_DELAY);
            }

            xSemaphoreGive( i2c0_semaphore );                               //release the mutex
        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, PAYLOAD1_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
