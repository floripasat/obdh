/*
 * imu_interface_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include <imu_interface_task.h>

void imuInterfaceTask( void *pvParameters )
{
    static uint8_t pucImu1Data[IMU_DATA_LENGTH];
    static uint8_t pucImu2Data[IMU_DATA_LENGTH];
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    volatile float fAccelX, fAccelY, fAccelZ, fAccelAbs;

    while(1)
    {
        //TODO: TASK ROUTINE
        vImuRead(pucImu1Data, IMU1);
//        vTaskDelay(5 / portTICK_PERIOD_MS); //delay between reads
//        vImuRead(pucImu2Data, IMU2);

        fAccelX = fImuRawToFloat(pucImu1Data[0], pucImu1Data[1]);
        fAccelY = fImuRawToFloat(pucImu1Data[2], pucImu1Data[3]);
        fAccelZ = fImuRawToFloat(pucImu1Data[4], pucImu1Data[5]);
        fAccelAbs = sqrtf(fAccelZ * fAccelZ + fAccelY * fAccelY + fAccelX * fAccelX);
        sprintf(imu_data, "IMU DATA: acX: %.2fg | acY: %.2fg | acz: %.2fg", fAccelX, fAccelY, fAccelZ);

        vTaskDelayUntil( &xLastWakeTime, IMU_INTERFACE_TASK_PERIOD_TICKS);
    }

    vTaskDelete( NULL );
}

