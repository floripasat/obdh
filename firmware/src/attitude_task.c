/*
 * attitude_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/attitude_task.h"

void prvAttitudeTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE


        vTaskDelayUntil( &xLastWakeTime, ATTITUDE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
