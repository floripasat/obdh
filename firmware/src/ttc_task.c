/*
 * ttc_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/ttc_task.h"

void prvTtcTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    static uint8_t usCounter = 20;

    while(1)
    {
        //TODO: TASK ROUTINE

        sprintf(ttc_data, "TT&C DATA: %u", usCounter);
        usCounter = 20 + (usCounter+1)%10;

        vTaskDelayUntil( &xLastWakeTime, TTC_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

