/*
 * read_temperature_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/read_temperature_task.h"

void prvReadTemperatureTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    volatile uint16_t usRaw;
    volatile float fVoltage, fTemp;


    while(1)
    {
        usRaw = adc_read();
        fVoltage = (usRaw * 1.5)/4095.0;
        fTemp = (float)(((long)usRaw - CALADC12_15V_30C) * (85.0 - 30.0)) / (CALADC12_15V_85C - CALADC12_15V_30C) + 30.0f;

        //TODO: TASK ROUTINE
        sprintf(temp_sens_data, "adc value(0-4095): %u, voltage(0-1.5): %.3fV -> temp: %.3f C", usRaw,fVoltage, fTemp);


        vTaskDelayUntil( &xLastWakeTime, READ_TEMPERATURE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

