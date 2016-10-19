/*
 * write_log_task.c
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#include "../include/write_log_task.h"

void prvWriteLogTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE


        vTaskDelayUntil( &xLastWakeTime, WRITE_LOG_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
