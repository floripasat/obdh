/*
 * payload_rush_interface_task.c
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
 * \file payload_rush_interface_task.c
 *
 * \brief Task that deals with the payload
 *
 * \author Elder Tramontin
 *
 */

#include "payload_rush_interface_task.h"

void rush_experiment_prepare( void );
void rush_experiment_log( void );
uint8_t rush_overtemperature_check( void );
void rush_delay_ms( uint8_t time_ms );
uint8_t rush_health_test( void );

uint8_t rush_data[RUSH_DATA_LENGTH];
uint32_t last_address_read = 0;

void payload_rush_interface_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();

    uint8_t last_address_update = 0;
    uint8_t energy_level;
    uint8_t rush_status = RUSH_POWER_OFF, rush_fpga_enable = RUSH_FPGA_DISABLE;

    rush_setup();

    while(1) {
        if (xSemaphoreTake( i2c0_semaphore, I2C_SEMAPHORE_WAIT_TIME ) == pdPASS) {    /**< try to get the mutex */

            energy_level = read_current_energy_level();

            switch (energy_level) {
            case ENERGY_L1_MODE:
                if(rush_status == RUSH_POWER_OFF) {        /**< if mode is power_off, turn it on   */
                    rush_status = RUSH_POWER_ON;
                    if( rush_overtemperature_check() ) {
                        rush_fpga_enable = RUSH_FPGA_ENABLE;
                    }
                }
                break;

            case ENERGY_L2_MODE:
                if(rush_status == RUSH_POWER_OFF){
                    rush_power_state(PAYLOAD_BOARD, TURN_ON);
                    rush_fpga_enable = RUSH_FPGA_DISABLE;
                }
                break;
            case ENERGY_L3_MODE:
            case ENERGY_L4_MODE:
            default:
                if(rush_status != RUSH_POWER_OFF) {        /**< if mode is power_on, turn it off    */
                    rush_status = RUSH_POWER_OFF;
                    rush_power_state(PAYLOAD_FPGA, TURN_OFF);
                    rush_power_state(PAYLOAD_BOARD, TURN_OFF);
                }
                break;
            }

            if(rush_status == RUSH_POWER_ON) {
                if ( last_address_update ) {
                    if (rush_fpga_enable == RUSH_FPGA_ENABLE) {
                        rush_power_state(PAYLOAD_FPGA, TURN_ON);
                        //TODO: Do the necessary delay to log data
                        rush_experiment_prepare();
                        rush_experiment_log();
                        rush_power_state(PAYLOAD_FPGA, TURN_OFF);
                    }
                }

                else {
                    /**< Set a start address to read */
                    rush_read((uint8_t *)&last_address_read, REG_LASTADDR, 4);
                    last_address_update = 1;
                }

                xSemaphoreGive( i2c0_semaphore );                  /**< release the mutex                   */
            }

            xQueueOverwrite(status_payload_rush_queue, &rush_status);

            if(rush_status == RUSH_POWER_ON) {
                xQueueSendToBack(payload_rush_queue, &rush_data, portMAX_DELAY);
            }
        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, PAYLOAD_RUSH_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

void rush_experiment_prepare( void ) {
    uint8_t setup_byte;
    uint32_t obdh_current_time;

    /**< Debug UART disabled */
    setup_byte = DEBUGEN_DISABLE_KEY;
    rush_write(&setup_byte, REG_DEBUGEN, 1);

    /**< Synchronize the payload with obdh current time */
    obdh_current_time = xTaskGetTickCount() / (uint32_t) configTICK_RATE_HZ;
    rush_write((uint8_t *)&obdh_current_time, REG_TIME, 4);

    /**< Set the scratch bit */
    setup_byte = STATUS_SCRATCH_1_MASK;
    rush_write(&setup_byte, REG_STATUS, 1);

    rush_delay_ms(500);
}

void rush_experiment_log( void ) {
    uint32_t new_log_address;
    int32_t blocks_to_read;

    /**< Check to see if there's new data to log */
    rush_read((uint8_t *)&new_log_address, REG_LASTADDR, 4);
    blocks_to_read = ((int32_t)new_log_address - (int32_t)last_address_read) / BLOCK_SIZE;

    /**< Only blocks by turn of read */
    if ( blocks_to_read > 0 ) {
        if (blocks_to_read > MAX_BLOCKS_PER_HEARTBEAT) {
            blocks_to_read = MAX_BLOCKS_PER_HEARTBEAT;
        }
        rush_read(rush_data, new_log_address, blocks_to_read);
        last_address_read += blocks_to_read;
    }
    else {
        rush_read((uint8_t *)&last_address_read, REG_LASTADDR, 4);
    }
    //TODO: Verify the MAX_BLOCK_PER_HEARTBEAT size and the last position update
}

uint8_t rush_overtemperature_check( void ) {
    uint8_t payload_status = PAYLOAD_NOT_OK;
    int16_t fpga_temp;

    rush_read((uint8_t *)&fpga_temp, REG_FPGATEMP, 2);
    if (fpga_temp > TEMPERATURE_HIGH_LIMIT) {
        payload_status = PAYLOAD_NOT_OK;
    } else if (fpga_temp < TEMPERATURE_LOW_LIMIT) {
        payload_status = PAYLOAD_OK;
    }

    return payload_status;
}

void rush_delay_ms( uint8_t time_ms ) {
    vTaskDelayMs(time_ms);
}

/**
 * \fn uint8_t rush_health_test( void )
 * Perform a build-in health test
 * \return health_test_result status result
 */
uint8_t rush_health_test( void ) {
    uint8_t tx_byte;
    uint32_t start_time;
    uint8_t health_test_result = 0;

    // Start the health test
    tx_byte = HEALTH_RUN_MASK;
    rush_write(&tx_byte, REG_HEALTH, 1);
    start_time = xTaskGetTickCount() / (uint32_t) configTICK_RATE_HZ;

    uint8_t i = 0;
    while ( ( xTaskGetTickCount() / (uint32_t) configTICK_RATE_HZ ) < ( start_time + HEALTH_TIMEOUT ) || i++ < HEALTH_TIMEOUT) {
        rush_read(&health_test_result, REG_HEALTH, 1);
        if (!(health_test_result & HEALTH_RUN_MASK)) {
            return health_test_result;
        }
        __delay_cycles(DELAY_1_S_IN_CYCLES);
    }

    // Timed out, try to stop test
    tx_byte = 0;
    rush_write(&tx_byte, REG_HEALTH, 1);

    return health_test_result;
}
