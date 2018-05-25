/*
 * payload1_interface_task.c
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
 * \file payload1_interface_task.c
 *
 * \brief Task that deals with the payload 1
 *
 * \author Elder Tramontin
 *
 */

#include "payload1_interface_task.h"

void payload1_experiment_prepare( void );
void payload1_experiment_log( void );
uint8_t payload1_overtemperature_check( void );
void payload1_delay_ms( uint8_t time_ms );

void payload1_interface_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();

    uint32_t last_address_read = 0;
    uint8_t last_address_update = 0;
    uint8_t energy_level;
    uint8_t payload1_data[PAYLOAD1_DATA_LENGTH];
    uint8_t payload1_status = PAYLOAD1_POWER_OFF;

    while(1) {
        if (xSemaphoreTake( i2c0_semaphore, I2C_SEMAPHORE_WAIT_TIME ) == pdPASS) {    /**< try to get the mutex */

            energy_level = read_current_energy_level();

            switch (energy_level) {
            case ENERGY_L1_MODE:
                if(payload1_status == PAYLOAD1_POWER_OFF) {        /**< if mode is power_off, turn it on   */
                    payload1_status = PAYLOAD1_POWER_ON;
                    payload1_power_state(PAYLOAD_BOARD, TURN_ON);
                }
                break;

            case ENERGY_L2_MODE:
            case ENERGY_L3_MODE:
            case ENERGY_L4_MODE:
            default:
                if(payload1_status != PAYLOAD1_POWER_OFF) {        /**< if mode is power_on, turn it off    */
                    payload1_status = PAYLOAD1_POWER_OFF;
                    payload1_power_state(PAYLOAD_FPGA, TURN_OFF);
                    payload1_power_state(PAYLOAD_BOARD, TURN_OFF);
                }
                break;
            }

            if(payload1_status == PAYLOAD1_POWER_ON) {
                if ( last_address_update ) {
                    payload1_experiment_prepare();
                    payload1_experiment_run();
                }
                else {
                    /**< Set a start address to read */
                    payload1_read((uint8_t *)&last_address_read, REG_LASTADDR, 4);
                    last_address_update = 1;
                }


                xSemaphoreGive( i2c0_semaphore );                  /**< release the mutex                   */
            }

            xQueueOverwrite(status_payload1_queue, &payload1_status);

            if(payload1_status == PAYLOAD1_POWER_ON) {
                xQueueSendToBack(payload1_queue, &payload1_data, portMAX_DELAY);
            }
        }

        if ( (last_wake_time + PAYLOAD1_INTERFACE_TASK_PERIOD_TICKS) < xTaskGetTickCount() ) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil( (TickType_t *) &last_wake_time, PAYLOAD1_INTERFACE_TASK_PERIOD_TICKS );
        }
    }

    vTaskDelete( NULL );
}

void payload1_experiment_prepare( void ) {
    uint8_t setup_byte;
    uint32_t obdh_current_time;

    /**< Debug UART disabled */
    setup_byte = DEBUGEN_DISABLE_KEY;
    payload1_write(&setup_byte, REG_DEBUGEN, 1);

    /**< Synchronize the payload with obdh current time */
    obdh_current_time = xTaskGetTickCount() / (uint32_t) configTICK_RATE_HZ;
    payload1_write((uint8_t *)&obdh_current_time, REG_TIME, 4);

    /**< Set the scratch bit */
    setup_byte = STATUS_SCRATCH_1_MASK;
    payload1_write(&setup_byte, REG_STATUS, 1);

    payload1_delay_ms(500);

    /**< fpga overtemperature protection */
    if(payload1_overtemperature_check()) {
        payload1_power_state(PAYLOAD_FPGA, TURN_ON);
    }
    else {
        payload1_power_state(PAYLOAD_FPGA, TURN_OFF);
    }

    payload1_delay_ms(500);
}

void payload1_experiment_log( void ) {
    uint32_t new_log_address;
    int32_t blocks_to_read;

    /**< Check to see if there's new data to log */
    payload1_read((uint8_t *)&new_log_address, REG_LASTADDR, 4);
    blocks_to_read = ((int32_t)new_log_address - (int32_t)last_address_read) / BLOCK_SIZE;

    /**< Only blocks by turn of read */
    if ( blocks_to_read > 0 ) {
        if (blocks_to_read > MAX_BLOCKS_PER_HEARTBEAT) {
            blocks_to_read = MAX_BLOCKS_PER_HEARTBEAT;
        }
        payload1_read(payload1_data, new_log_address, blocks_to_read);
        last_address_read += blocks_to_read;
    }
    else {
        payload1_read((uint8_t *)&last_address_read, REG_LASTADDR, 4);
    }

    payload1_power_state(PAYLOAD_FPGA, TURN_OFF);
}

uint8_t payload1_overtemperature_check( void ) {
    uint8_t payload_status = PAYLOAD_NOT_OK;
    int16_t fpga_temp;

    payload1_read((uint8_t *)&fpga_temp, REG_FPGATEMP, 2);
    if (fpga_temp > TEMPERATURE_HIGH_LIMIT) {
        payload_status = PAYLOAD_NOT_OK;
    } else if (fpga_temp < TEMPERATURE_LOW_LIMIT) {
        payload_status = PAYLOAD_OK;
    }

    return payload_status;
}

void payload1_delay_ms( uint8_t time_ms ) {
    vTaskDelayMs(time_ms);
}
