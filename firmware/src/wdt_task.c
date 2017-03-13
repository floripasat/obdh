/*
 * wdt_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include <wdt_task.h>

void wdt_task( void *pvParameters )
{
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();

    while(1)
    {
        wdti_reset_counter();
        wdte_reset_counter();

        vTaskDelayUntil( (TickType_t *) &last_wake_time, WDT_TASK_PERIOD_TICKS);
    }

    vTaskDelete( NULL );
}

