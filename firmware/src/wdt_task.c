/*
 * wdt_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include <wdt_task.h>

void wdtTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE
        wdti_reset_counter();
        wdte_reset_counter();

        vTaskDelayUntil( (TickType_t *) &xLastWakeTime, WDT_TASK_PERIOD_TICKS);
    }

    vTaskDelete( NULL );
}

