/*
 * imu_interface_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include <imu_interface_task.h>

void imu_interface_task( void *pvParameters )
{
    uint8_t imu_data_temp[20];

    TickType_t last_wake_time;

    float accelerometer_x, accelerometer_y, accelerometer_z, accelerometer_absolute;
//    float gyroscope_x, gyroscope_y, gyroscope_z;
//    float magnetometer_x, magnetometer_y, magnetometer_z;
//    float temperature;

    //Set IMU pins
    imu_setup();

    last_wake_time = xTaskGetTickCount();

    while(1) {
        imu_read(imu_data_temp, IMU1);

        accelerometer_x = imu_acc_raw_to_g(imu_data_temp[0], imu_data_temp[1]);
        accelerometer_y = imu_acc_raw_to_g(imu_data_temp[2], imu_data_temp[3]);
        accelerometer_z = imu_acc_raw_to_g(imu_data_temp[4], imu_data_temp[5]);
        accelerometer_absolute = sqrtf(accelerometer_z * accelerometer_z + accelerometer_y * accelerometer_y + accelerometer_x * accelerometer_x);

//        sprintf((char *)imu_data, "IMU DATA: acX: %.2fg | acY: %.2fg | acz: %.2fg", accelerometer_x, accelerometer_y, accelerometer_z);

        for(int i = 0; i < sizeof(satellite_data.imu); i++)
            satellite_data.imu[i] = imu_data_temp[i];

        vTaskDelayUntil( (TickType_t *) &last_wake_time, IMU_INTERFACE_TASK_PERIOD_TICKS);
    }

    vTaskDelete( NULL );
}

