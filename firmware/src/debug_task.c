/*
 * debug_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/debug_task.h"

void prvDebugTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE

uart_tx("")
        vTaskDelayUntil( &xLastWakeTime, DEBUG_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
