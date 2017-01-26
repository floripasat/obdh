/*
 * ttc_interface_task.c
 *
 *  Created on: 18 de jan de 2017
 *      Author: elder
 */

#include "../include/ttc_interface_task.h"

void ttcInterfaceTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    static uint8_t usCounter = 5;

    while(1)
    {
        //TODO: TASK ROUTINE

        sprintf(ttc_data,"TTC DATA: %u ",usCounter);
        usCounter = (usCounter+1)%10;

        vTaskDelayUntil( &xLastWakeTime, TTC_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
