/*
 * eps_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/eps_task.h"

void prvEpsTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE

        vTaskDelayUntil( &xLastWakeTime, EPS_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
