/*
 * antenna_task.c
 *
 *  Created on: 04 de nov de 2016
 *      Author: elder
 */

#include "../include/antenna_task.h"

void antennaTask( void *pvParameters )
{
//    volatile TickType_t xLastWakeTime;
//    xLastWakeTime = xTaskGetTickCount();


    //TODO: TASK ROUTINE
    vTaskDelay(2700000 / portTICK_PERIOD_MS); //45 min * 60s * 1000ms = 2.700.000 ms

    vAntennaDeploy();

//        vTaskDelayUntil( &xLastWakeTime, ANTENNA_TASK_PERIOD_TICKS );


    vTaskDelete( NULL );
}
