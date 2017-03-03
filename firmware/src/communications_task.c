/*
 * communications_task.c
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#include <communications_task.h>

void communications_task( void *pvParameters )
{
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    uint16_t usCount;

    while(1)
    {
        //TODO: TASK ROUTINE
        for(usCount = 0; usCount < 10000; usCount++)
        {

        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, COMMUNICATIONS_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
