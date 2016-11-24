/*
 * solar_panels_interface_task.c
 *
 *  Created on: 04 de nov de 2016
 *      Author: elder
 */

#include <solar_panels_interface_task.h>

void solarPanelsInterfaceTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    uint16_t usCount;

    while(1)
    {
        //TODO: TASK ROUTINE
        for(usCount = 0; usCount < 10000; usCount++)
        {

        }

        vTaskDelayUntil( &xLastWakeTime, SOLAR_PANELS_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
