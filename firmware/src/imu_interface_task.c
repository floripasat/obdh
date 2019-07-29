/*
 * imu_interface_task.c
 *
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina
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
 * \brief Task that deals with the IMUs
 *
 * \author Elder Tramontin
 *
 * \version 0.2.9
 *
 * \addtogroup imu_interface_task
 * \{
 */

#include "../interface/debug/debug.h"

#include "imu_interface_task.h"

void imu_interface_task(void *pvParameters) {
    debug_print_event_from_module(DEBUG_INFO, "Tasks", "Initializing IMU task...");
    debug_new_line();

    uint8_t imu_data_temp[14];
    uint8_t imu_data[24];
    uint8_t module_test;
    uint8_t imu_status;
    uint8_t turn = 0;
    uint8_t i;
    TickType_t last_wake_time;

#ifdef _DEBUG
    volatile float accelerometer_x, accelerometer_y, accelerometer_z, accelerometer_absolute;
    volatile float gyroscope_x, gyroscope_y, gyroscope_z, gyroscope_absolute_module;
    volatile float temperature;
#endif

    module_test = imu_setup();                          // Setup IMU pins and verify the communication

    if (module_test == IMU_NOT_WORKING)
    {
        debug_print_event_from_module(DEBUG_ERROR, "IMU Task", "Main IMU not working!");
        debug_new_line();

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

        // Use 8-bit IMU and save 2 samples/packet (sampling rate = 2Hz)
        if (turn == 0) {                                // first sampling
            for(i = 0; i < 6; i++) {
                imu_data[i] = imu_data_temp[i];         // accelerometer data
            }
            for(i = 6; i < 12; i++) {
                imu_data[i] = imu_data_temp[i+2];       // gyroscope data
            }

            turn = 1;
        }
        else {                                          // second sampling
            for(i = 0; i < 6; i++) {
                imu_data[i+12] = imu_data_temp[i];      // accelerometer data
            }
            for(i = 6; i < 12; i++) {
                imu_data[i+12] = imu_data_temp[i+2];    // gyroscope data
            }

            xQueueOverwrite(status_imu_queue, &imu_status);

            if (imu_status == IMU_WORKING) {
                xQueueSendToBack(imu_queue, imu_data, portMAX_DELAY);
            }

            turn = 0;
        }

        if ((last_wake_time + IMU_INTERFACE_TASK_PERIOD_TICKS) < xTaskGetTickCount()) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil((TickType_t *) &last_wake_time, IMU_INTERFACE_TASK_PERIOD_TICKS);
        }
    }

    vTaskDelete(NULL);
}

//! \} End of imu_interface_task group
