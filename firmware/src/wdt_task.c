/*
 * wdt_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/wdt_task.h"

void prvWdtTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    //ENABLE WDTs


    while(1)
    {
        //TODO: TASK ROUTINE

        P5OUT |= BIT4; //set pin

        vTaskDelayMs( 10 );

        P5OUT ^= BIT4; //clear pin


        vTaskDelayUntil( &xLastWakeTime, WDT_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

