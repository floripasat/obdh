/*
 * save_data_on_flash_memory_task.c
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#include "../include/save_data_on_flash_memory_task.h"

void saveDataOnFlashMemoryTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE


        vTaskDelayUntil( &xLastWakeTime, SAVE_DATA_ON_FLASH_MEMORY_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
