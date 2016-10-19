/*
 * solar_panel_task.c
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#include "../include/solar_panel_task.h"

void prvSolarPanelTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE


        vTaskDelayUntil( &xLastWakeTime, SOLAR_PANEL_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
