/*
 * imu_interface_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include <imu_interface_task.h>

void imuInterfaceTask( void *pvParameters )
{
    static uint8_t imu_data_temp[IMU_DATA_LENGTH];

    volatile TickType_t xLastWakeTime;
    volatile float fAccelX, fAccelY, fAccelZ, fAccelAbs;

    //Set IMU pins
    imu_setup();

    //    vI2cSetup(IMU_BASE_ADDRESS,MPU0_I2C_SLAVE_ADRESS);
    //
    //    vImuConfig();

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        imu_read(imu_data_temp, IMU1);

        fAccelX = imu_acc_raw_to_g(imu_data_temp[0], imu_data_temp[1]);
        fAccelY = imu_acc_raw_to_g(imu_data_temp[2], imu_data_temp[3]);
        fAccelZ = imu_acc_raw_to_g(imu_data_temp[4], imu_data_temp[5]);
        fAccelAbs = sqrtf(fAccelZ * fAccelZ + fAccelY * fAccelY + fAccelX * fAccelX);

        sprintf((char *)imu_data, "IMU DATA: acX: %.2fg | acY: %.2fg | acz: %.2fg", fAccelX, fAccelY, fAccelZ);

//        for(int i = 0; i < 14; i++)
//            imu_data[i] = imu_data_temp[i];

        vTaskDelayUntil( (TickType_t *) &xLastWakeTime, IMU_INTERFACE_TASK_PERIOD_TICKS);
    }

    vTaskDelete( NULL );
}

