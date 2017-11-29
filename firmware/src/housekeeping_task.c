/*
 * housekeeping_task.c
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
 * \file housekeeping_task.c
 *
 * \brief Task that do the housekeeping: update time counter,
 * read the supply voltage and current and manage the operation modes
 *
 * \author Elder Tramontin
 *
 */

#include "../src/housekeeping_task.h"


void housekeeping_task( void *pvParameters ) {
    TickType_t last_wake_time;
    uint16_t temperature_raw, voltage_raw, current_raw;
#ifdef _DEBUG
    volatile float temperature, voltage, current;
#endif
    uint8_t internal_sensors_data[6];
    uint8_t system_status[6];
    uint32_t reset_value;
    uint8_t temp_status_flags, status_flags;
    uint8_t current_mode;
    uint32_t system_time, time_state_last_change;
    uint32_t current_time;
    uint8_t current_seconds;
    uint8_t flag_updated_time = 0;

    last_wake_time = xTaskGetTickCount();

    while(1) {
        /* Periodic reset */
        current_time = xTaskGetTickCount() / (uint32_t) configTICK_RATE_HZ;
        if (current_time >= PERIODIC_RESET_TIME) {
            system_reboot();
        }

        /* read internal temperature */
        temperature_raw = obdh_temperature_read();

        /* read supply voltage */
        voltage_raw = obdh_voltage_read();

        /* read supply current */
        current_raw = obdh_current_read();

        internal_sensors_data[0] = temperature_raw>>8 & 0xFF;
        internal_sensors_data[1] = temperature_raw & 0xFF;
        internal_sensors_data[2] = voltage_raw>>8 & 0xFF;
        internal_sensors_data[3] = voltage_raw & 0xFF;
        internal_sensors_data[4] = current_raw>>8 & 0xFF;
        internal_sensors_data[5] = current_raw & 0xFF;

#ifdef _DEBUG

        voltage = obdh_voltage_convert(voltage_raw);
        temperature = obdh_temperature_convert(temperature_raw);
        current = obdh_current_convert(current_raw);

        satellite_data.msp_sensors[0] = internal_sensors_data[0];
        satellite_data.msp_sensors[1] = internal_sensors_data[1];
        satellite_data.msp_sensors[2] = internal_sensors_data[2];
        satellite_data.msp_sensors[3] = internal_sensors_data[3];
        satellite_data.msp_sensors[4] = internal_sensors_data[4];
        satellite_data.msp_sensors[5] = internal_sensors_data[5];
#endif

        /* Receive modules status
         * Read reset counter
         * Read fault flags
         * */

        status_flags = 0;
        temp_status_flags = 0;
        if(xQueuePeek(status_eps_queue, &temp_status_flags, 0) == pdPASS) {
            status_flags |= temp_status_flags<<0;
        }
        if(xQueuePeek(status_payload1_queue, &temp_status_flags, 0) == pdPASS) {
            status_flags |= temp_status_flags<<1;
        }
        if(xQueuePeek(status_payload2_queue, &temp_status_flags, 0) == pdPASS) {
            status_flags |= temp_status_flags<<2;
        }
        if(xQueuePeek(status_mem1_queue, &temp_status_flags, 0) == pdPASS) {
            status_flags |= temp_status_flags<<3;
        }
        if(xQueuePeek(status_imu_queue, &temp_status_flags, 0) == pdPASS) {
            status_flags |= temp_status_flags<<4;
        }

        reset_value = read_reset_value();
        system_status[0] = reset_value>>16  & 0xFF;
        system_status[1] = reset_value>>8   & 0xFF;
        system_status[2] = reset_value      & 0xFF;
        system_status[3] = reset_value>>24  & 0xFF;     /**< reset cause        */
        system_status[4] = read_fault_flags();          /**< clock fault flags  */
        system_status[5] = status_flags;

        current_mode = read_current_operation_mode();
        current_seconds =  (xTaskGetTickCount() / (uint32_t) configTICK_RATE_HZ) % 60; /**< add the seconds to the lower byte  */

        if( current_seconds == 0 && flag_updated_time == 0) {/**< if 1 minute has passed     */
            xSemaphoreTake(flash_semaphore, FLASH_SEMAPHORE_WAIT_TIME); /**< protect the flash from mutual access */
            update_time_counter();                           /**< update the minutes counter */
            xSemaphoreGive(flash_semaphore);
            flag_updated_time = 1;
        }
        /**< to prevent counting twice the same minute (if the task period was less than 1s */
        else {
            flag_updated_time = 0;
        }

        system_time = read_time_counter();

        if(current_mode  == SHUTDOWN_MODE) {
            time_state_last_change = read_time_state_changed();
            if( system_time - time_state_last_change >= (MINUTES_IN_A_DAY)  ) {
                xSemaphoreTake(flash_semaphore, FLASH_SEMAPHORE_WAIT_TIME);/**< protect the flash from mutual access */
                update_operation_mode(NORMAL_OPERATION_MODE);
                xSemaphoreGive(flash_semaphore);
            }
        }

                                                          /**< shift minutes to the 24 most significant bits */
        system_time = (system_time<<24 & 0xFF000000) |    /**< to agree with the big-endian format           */
                      (system_time<<8  & 0x00FF0000) |
                      (system_time>>8  & 0x0000FF00) |
                      current_seconds;                    /**< seconds stored in the LSB                     */

        xQueueSendToBack(system_status_queue, (void *)system_status, portMAX_DELAY);

        xQueueSendToBack(system_time_queue, (void *)&system_time, portMAX_DELAY);

        xQueueSendToBack(internal_sensors_queue, (void *)internal_sensors_data, portMAX_DELAY);
        vTaskDelayUntil( (TickType_t *) &last_wake_time, HOUSEKEEPING_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

