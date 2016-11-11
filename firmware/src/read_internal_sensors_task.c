/*
 * read_internal_sensors_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/read_internal_sensors_task.h"

void readInternalSensorsTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    volatile uint16_t usTempRaw;
    volatile float fTemp;

    while(1)
    {
        usTempRaw = obdh_temp_read();
        fTemp = obdh_temp_convert(usTempRaw);

        //TODO: TASK ROUTINE
        sprintf(temp_sens_data, "adc value(0-4095): %u -> temp: %.3f C", usTempRaw, fTemp);

        vTaskDelayUntil( &xLastWakeTime, READ_INTERNAL_SENSORS_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

