/*
 * imu_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/imu_task.h"

void prvImuTask( void *pvParameters )
{
    static uint8_t pucImu1Data[IMU_DATA_LENGTH];
    static uint8_t pucImu2Data[IMU_DATA_LENGTH];
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    static uint8_t usCounter = 10;

    while(1)
    {
        //TODO: TASK ROUTINE
//        vImuRead(pucImu1Data, IMU1);
//        vTaskDelay(5 / portTICK_PERIOD_MS); //delay between reads
//        vImuRead(pucImu2Data, IMU2);

        sprintf(imu_data, "IMU DATA: %u", usCounter);
        usCounter = 10 + (usCounter+1)%10;

        vTaskDelayUntil( &xLastWakeTime, IMU_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

