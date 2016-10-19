/*
 * send_down_data_task.c
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#include "../include/send_down_data_task.h"

void prvSendDownDataTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE


        vTaskDelayUntil( &xLastWakeTime, SEND_DOWN_DATA_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
