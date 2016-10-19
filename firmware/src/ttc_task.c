/*
 * ttc_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/ttc_task.h"

void prvTtcTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE

        vTaskDelayUntil( &xLastWakeTime, TTC_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

