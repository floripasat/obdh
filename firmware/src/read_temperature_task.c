/*
 * read_temperature_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/read_temperature_task.h"

void prvReadTemperatureTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE

        vTaskDelayUntil( &xLastWakeTime, READ_TEMPERATURE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

