/*
 * ttc_interface_task.c
 *
 *  Created on: 18 de jan de 2017
 *      Author: elder
 */

#include <ttc_interface_task.h>

void ttc_interface_task( void *pvParameters )
{
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();

    ttc_setup();

    while(1)
    {
        //TODO: TASK ROUTINE
//        ttc_read( );

        vTaskDelayUntil( (TickType_t *) &last_wake_time, TTC_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
