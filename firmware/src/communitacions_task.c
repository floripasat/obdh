/*
 * communications_task.c
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#include "../include/communications_task.h"

void communicationsTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE


        vTaskDelayUntil( &xLastWakeTime, COMMUNICATIONS_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
