/*
 * debug_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/debug_task.h"

void debugTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    static char uart_package[200];
    while(1)
    {
        //TODO: TASK ROUTINE
        /* assemble the package */
        sprintf(uart_package, "%s\n%s\n%s\n%s\n\n", eps_data, imu_data, ttc_data, temp_sens_data);

        /* send the package */
        uart_tx(uart_package);


        vTaskDelayUntil( &xLastWakeTime, DEBUG_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
