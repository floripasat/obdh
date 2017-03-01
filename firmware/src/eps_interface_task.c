/*
 * epsInterface_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include <eps_interface_task.h>

void eps_interface_task( void *pvParameters )
{
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    eps_package_t eps_package;

    eps_setup();

    while(1)
    {
        //TODO: TASK ROUTINE

        eps_read(&eps_package);

        vTaskDelayUntil( (TickType_t *) &last_wake_time, EPS_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
