/*
 * solar_panels_interface_task.c
 *
 *  Created on: 04 de nov de 2016
 *      Author: elder
 */

#include <solar_panels_interface_task.h>

void solar_panels_interface_task( void *pvParameters )
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

        vTaskDelayUntil( (TickType_t *) &last_wake_time, SOLAR_PANELS_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
