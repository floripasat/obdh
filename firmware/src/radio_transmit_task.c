/*
 * radio_transmit_task.c
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#include "../include/radio_transmit_task.h"

void prvRadioTransmitTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE


        vTaskDelayUntil( &xLastWakeTime, RADIO_TRANSMIT_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
