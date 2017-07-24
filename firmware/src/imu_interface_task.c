/*
 * imu_interface_task.c
 *
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
 *
 * This file is part of FloripaSat-OBDH.
 *
 * FloripaSat-OBDH is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FloripaSat-OBDH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /**
 * \file imu_interface_task.c
 *
 * \brief Task that deals with the IMUs
 *
 * \author Elder Tramontin
 *
 */

#include "imu_interface_task.h"

void imu_interface_task( void *pvParameters ) {
    uint8_t imu_data_temp[20];
    uint8_t module_test;
    uint8_t imu_status;
    TickType_t last_wake_time;

#ifdef _DEBUG
    float accelerometer_x, accelerometer_y, accelerometer_z, accelerometer_absolute;
    float gyroscope_x, gyroscope_y, gyroscope_z, gyroscope_absolute_module;
    float temperature;
#endif

    //Set IMU pins and verify the communication
    module_test = imu_setup();
    if (module_test == IMU_NOT_WORKING)
    {
        //TODO:  use another IMU
    }
    last_wake_time = xTaskGetTickCount();

    while(1) {
        imu_status = imu_read(imu_data_temp, IMU1);

#ifdef _DEBUG
        accelerometer_x = imu_acc_raw_to_g(imu_data_temp[0], imu_data_temp[1]);
        accelerometer_y = imu_acc_raw_to_g(imu_data_temp[2], imu_data_temp[3]);
        accelerometer_z = imu_acc_raw_to_g(imu_data_temp[4], imu_data_temp[5]);
        accelerometer_absolute = sqrtf(accelerometer_z * accelerometer_z + accelerometer_y * accelerometer_y + accelerometer_x * accelerometer_x);

        temperature = imu_temp_raw_to_degrees(imu_data_temp[6], imu_data_temp[7]);

        gyroscope_x = imu_gyr_raw_to_dps(imu_data_temp[8], imu_data_temp[9]);
        gyroscope_y = imu_gyr_raw_to_dps(imu_data_temp[10], imu_data_temp[11]);
        gyroscope_z = imu_gyr_raw_to_dps(imu_data_temp[12], imu_data_temp[13]);
        gyroscope_absolute_module = sqrtf(gyroscope_z * gyroscope_z + gyroscope_y * gyroscope_y + gyroscope_x * gyroscope_x);
#endif

        xQueueOverwrite(status_imu_queue, &imu_status);

        if(imu_status == IMU_WORKING) {
            xQueueSendToBack(imu_queue, imu_data_temp, portMAX_DELAY);
        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, IMU_INTERFACE_TASK_PERIOD_TICKS);
    }

    vTaskDelete( NULL );
}

