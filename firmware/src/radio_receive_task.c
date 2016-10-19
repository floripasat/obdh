/*
 * radio_receive_task.c
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#include "../include/radio_receive_task.h"

void prvRadioReceiveTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE


        vTaskDelayUntil( &xLastWakeTime, RADIO_RECEIVE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
