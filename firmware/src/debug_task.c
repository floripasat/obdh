/*
 * debug_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include <debug_task.h>

void debugTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    static char uart_package[400];
    static char cmd[10];
    static char oldCmd = 0;
    volatile uint32_t systick;
    while(1)
    {
        //TODO: TASK ROUTINE
        /* assemble the package */
//        sprintf(uart_package, "%s\n%s\n%s\n%s\n\n", eps_data, imu_data, ttc_data, temp_sens_data);
        /* send the package */
//        uart_tx(uart_package);

        uart_rx(cmd, 1);

        switch(cmd[0])
        {
            /* GENERAL INFO/STATUS */
            case '1':   sprintf(uart_package, "Last compilation: v0.38 \r\nLast cmd: %c\r\n%s\r\n%s",oldCmd,  eps_data, msp_internal_data);
            break;
            /* BUS VOLTAGES */
            case '2':   sprintf(uart_package, "BUS VOLTAGE: ");
            break;
            /* IMU READ */
            case '3':   sprintf(uart_package, "%s", imu_data);
            break;
            /* MEMORY DUMP */
            case '4':   sprintf(uart_package, "Memory dumped");
            break;
            /* MEMORY CLEAR */
            case '5':  sprintf(uart_package, "Memory cleared");
            break;
            case '6': systick = xTaskGetTickCount();
                sprintf(uart_package, "Systick: %lu \n\rhours: %f \n\rminutes: %f", systick, systick/3600000.0, systick/60000.0);
            break;
            default:   sprintf(uart_package, "Invalid request: %c",cmd[0]);
        }
        oldCmd = cmd[0];

        uart_tx("{");               //send start of frame
        uart_tx(uart_package);
        uart_tx("}\r\n");           //send stop of frame

        vTaskDelayUntil( (TickType_t *) &xLastWakeTime, DEBUG_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

