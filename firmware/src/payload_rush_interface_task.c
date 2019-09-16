/*
 * payload_rush_interface_task.c
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
 * \brief Task that deals with the payload
 *
 * \author Andre Mattos
 *
 * \version 0.2.12
 *
 * \addtogroup rush_task
 * \{
 */

#include "../interface/debug/debug.h"

#include "payload_rush_interface_task.h"

void rush_experiment_perform(void);
uint8_t rush_experiment_log(void);
void rush_delay_ms(uint16_t time_ms);
uint8_t rush_overtemperature_check(void);
uint8_t rush_health_test(void);

uint8_t rush_data[RUSH_DATA_LENGTH];
static uint32_t last_data_address = 0;
uint32_t current_data_address;

void payload_rush_interface_task(void *pvParameters) {
    debug_print_event_from_module(DEBUG_INFO, "Tasks", "Initializing RUSH tasks...");
    debug_new_line();

    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();

    uint8_t last_address_update = 0;
    uint8_t energy_level;
//    uint16_t iteration_counter = 0;
    uint8_t read_attempts = COMMUNICATION_MAX_ATTEMPTS;
    uint8_t rush_status = RUSH_POWER_OFF;
    volatile uint8_t rush_fpga_enable = RUSH_FPGA_DISABLE;

    //stores a command received by the command_to_payload_rush_queue. The command is the amount
    //of time (in minutes) to turns the RUSH on and to perform an experiment.
    uint8_t command_received = 0;
    //stores the total duration of an experiment for timing purposes.
    uint16_t experiment_duration = 0;
    //time counter of an already running experiment in seconds.
    uint16_t running_experiment_counter = 0;

    rush_setup();

    while(1)
    {
        // try to get the mutex
        if (xSemaphoreTake(i2c0_semaphore, I2C_SEMAPHORE_WAIT_TIME) == pdPASS)
        {
            //waiting the end of the current experiment to receive a new command to start a new experiment.
            //running_experiment_counter will be decremented by 1 each task cycle (1 second) until it
            //reaches 0, so a new command could be received via the command_to_payload_rush_queue.
            if (running_experiment_counter == 0)
            {
                //checks if there is some new command in the queue.
                if (xQueueReceive(command_to_payload_rush_queue,&command_received, 0) == pdTRUE)
                {
                    //based on the new command_received from the queue, running_experiment_counter will be
                    //updated with a new value to start/timer a new experiment. A redundant range check will
                    //be performed.
                    if (command_received == 0)
                    {
                        running_experiment_counter = 0;
                    }
                    else if ((command_received >= MINIMUM_EXPERIMENT_TIME)&(command_received <= MAXIMUM_EXPERIMENT_TIME))
                    {
                        running_experiment_counter = command_received*60; //conversion to seconds, command_received stores the time in minutes.
                    }
                    else
                    {
                        running_experiment_counter = DEFAULT_EXPERIMENT_TIME*60; //convertion to seconds.
                    }
                    //update de experiment_duration register for timing purporses
                    experiment_duration = running_experiment_counter;
                }
            }
            //experiment running
            else
            {
                running_experiment_counter--;

                if (rush_status == RUSH_POWER_ON)
                {
                    //verify if some command to finish the experiment was sent.
                    if (xQueuePeek(command_to_payload_rush_queue,&command_received, 0) == pdTRUE)
                    {
                        //if yes, finish the experiment
                        if (command_received == 0)
                        {
                            xQueueReset(command_to_payload_rush_queue);
                            running_experiment_counter = 0;
                            rush_fpga_enable = RUSH_FPGA_DISABLE;
                        }
                    }

                    if (last_address_update)
                    {
                        if ((rush_fpga_enable == RUSH_FPGA_ENABLE) && rush_overtemperature_check())
                        {
                            //turns the FPGA on after EXPERIMENT_START_ITERATION seconds from experiment begin
                            if (running_experiment_counter == experiment_duration - TIME_TO_TURN_FPGA_ON)
                            {
                                rush_power_state(PAYLOAD_FPGA, TURN_ON);
                                //!!!!!acho que esta função deveria ficar aqui e não no final do experimento!!!!!!
                                rush_experiment_perform();
                            }
                            if (running_experiment_counter == 0)
                            {
                                rush_power_state(PAYLOAD_FPGA, TURN_OFF);
                                rush_fpga_enable = RUSH_FPGA_DISABLE;
                            }
                        }
                        else
                        {
                            rush_power_state(PAYLOAD_FPGA, TURN_OFF);
                            rush_fpga_enable = RUSH_FPGA_DISABLE;
                            running_experiment_counter = 0;
                        }
                    }
                    else
                    {
                        // Set a start address to read
                        while((rush_read((uint8_t *)&last_data_address, REG_LASTADDR, 4) == RUSH_COMM_ERROR) && (read_attempts-- != 0));
                        last_address_update = 1;
                    }

                    //verify if there is some data to read in RUSH`s memmory
                    if (rush_experiment_log() != 0)
                    {
                        //if there is, send to the data queue.
                        xQueueSendToBack(payload_rush_queue, &rush_data, portMAX_DELAY);
                    }
                    //if there isn`t and the is the end of the experiment, turn all off.
                    else if (running_experiment_counter == 0)
                    {
                        if (rush_status != RUSH_POWER_OFF)          // if mode is power_on, turn it off
                        {
                            rush_power_state(PAYLOAD_FPGA, TURN_OFF);
                            rush_fpga_enable = RUSH_FPGA_DISABLE;
                            rush_power_state(PAYLOAD_BOARD, TURN_OFF);
                            rush_status = RUSH_POWER_OFF;
                        }
                    }
                }
            }

            energy_level = read_current_energy_level();

            switch(energy_level) {
                case ENERGY_L1_MODE:
                    if (running_experiment_counter != 0)
                    {
                        if (rush_status == RUSH_POWER_OFF) {        // if mode is power_off, turn it on
                            rush_power_state(PAYLOAD_BOARD, TURN_ON);
                            rush_status = RUSH_POWER_ON;
                        }
                        if ((rush_fpga_enable == RUSH_FPGA_DISABLE ) && rush_overtemperature_check()) {
                            rush_fpga_enable = RUSH_FPGA_ENABLE;
                        }
                    }
                    break;

                case ENERGY_L2_MODE:
//                    if (rush_status == RUSH_POWER_OFF) {
//                        rush_power_state(PAYLOAD_BOARD, TURN_ON);
//                        rush_fpga_enable = RUSH_FPGA_DISABLE;
//                    }
//                    break;
                case ENERGY_L3_MODE:
                case ENERGY_L4_MODE:
                default:
                    if (rush_status != RUSH_POWER_OFF) {        // if mode is power_on, turn it off
                        running_experiment_counter = 0;
                        rush_power_state(PAYLOAD_FPGA, TURN_OFF);
                        rush_status = RUSH_POWER_OFF;
                        rush_power_state(PAYLOAD_BOARD, TURN_OFF);
                        rush_fpga_enable = RUSH_FPGA_DISABLE;
                        xQueueReset(command_to_payload_rush_queue);
                    }
                    break;
            }

            xSemaphoreGive(i2c0_semaphore);                     // release the mutex

            xQueueOverwrite(status_payload_rush_queue, &rush_status);
        }

        vTaskDelayUntil((TickType_t *) &last_wake_time, PAYLOAD_RUSH_INTERFACE_TASK_PERIOD_TICKS);
    }
//    while(1) {
//        if ( xSemaphoreTake( i2c0_semaphore, I2C_SEMAPHORE_WAIT_TIME ) == pdPASS) {    /**< try to get the mutex */
//
//            energy_level = read_current_energy_level();
//
//            switch ( energy_level ) {
//            case ENERGY_L1_MODE:
//                if( rush_status == RUSH_POWER_OFF ) {        /**< if mode is power_off, turn it on   */
//                    rush_power_state(PAYLOAD_BOARD, TURN_ON);
//                    rush_status = RUSH_POWER_ON;
//                }
//                if ( ( rush_fpga_enable == RUSH_FPGA_DISABLE ) && rush_overtemperature_check() ) {
//                    rush_fpga_enable = RUSH_FPGA_ENABLE;
//                }
//                break;
//
//            case ENERGY_L2_MODE:
//                if( rush_status == RUSH_POWER_OFF ){
//                    rush_power_state(PAYLOAD_BOARD, TURN_ON);
//                    rush_fpga_enable = RUSH_FPGA_DISABLE;
//                }
//                break;
//            case ENERGY_L3_MODE:
//            case ENERGY_L4_MODE:
//            default:
//                if ( rush_status != RUSH_POWER_OFF ) {        /**< if mode is power_on, turn it off    */
//                    rush_status = RUSH_POWER_OFF;
//                    rush_power_state(PAYLOAD_FPGA, TURN_OFF);
//                    rush_power_state(PAYLOAD_BOARD, TURN_OFF);
//                }
//                break;
//            }
//
//            if( rush_status == RUSH_POWER_ON ) {
//                if ( last_address_update ) {
//                    if ( ( rush_fpga_enable == RUSH_FPGA_ENABLE ) && rush_overtemperature_check() ) {
//                        if (iteration_counter == EXPERIMENT_START_ITERATION) {
//                            rush_power_state(PAYLOAD_FPGA, TURN_ON);
//                        }
//
//                        if ( iteration_counter++ >= EXPERIMENT_END_ITERATION ) {
//                            rush_experiment_perform();
//                            rush_power_state(PAYLOAD_FPGA, TURN_OFF);
//                            rush_experiment_log();
//                            iteration_counter = 0;
//                            xQueueSendToBack(payload_rush_queue, &rush_data, portMAX_DELAY);
//                        }
//                    }
//                    else {
//                        rush_power_state(PAYLOAD_FPGA, TURN_OFF);
//                        rush_fpga_enable == RUSH_FPGA_DISABLE;
//                        iteration_counter = 0;
//                    }
//                }
//
//                else {
//                    /**< Set a start address to read */
//                    while( ( rush_read( (uint8_t *)&last_data_address, REG_LASTADDR, 4 ) == RUSH_COMM_ERROR ) && ( read_attempts-- != 0 ) );
//                    last_address_update = 1;
//                }
//            }
//
//            xSemaphoreGive( i2c0_semaphore );                  /**< release the mutex */
//
//            xQueueOverwrite(status_payload_rush_queue, &rush_status);
//        }
//
//        vTaskDelayUntil( (TickType_t *) &last_wake_time, PAYLOAD_RUSH_INTERFACE_TASK_PERIOD_TICKS );
//    }

    vTaskDelete( NULL );
}

/**
 * \brief Prepare rush to perform an experiment.
 *
 * \return none
 */
void rush_experiment_perform(void) {
    uint8_t setup_byte;
    uint32_t obdh_current_time;

    // Debug UART disabled
    setup_byte = DEBUGEN_DISABLE_KEY;
    rush_write(&setup_byte, REG_DEBUGEN, 1);

    // Synchronize the payload with obdh current time
    obdh_current_time = xTaskGetTickCount() / (uint32_t) configTICK_RATE_HZ;
    rush_write((uint8_t *)&obdh_current_time, REG_TIME, 4);

    // Set the scratch bit
    setup_byte = STATUS_SCRATCH_1_MASK;
    rush_write(&setup_byte, REG_STATUS, 1);

    rush_delay_ms(RESPONSE_TIME_MS);
}

/**
 * \brief Read and update the last data packet.
 *
 * \return none
 */
uint8_t rush_experiment_log(void) {
    uint8_t read_attempts = COMMUNICATION_MAX_ATTEMPTS;
    uint32_t data_length;

    // Get the current data address
    while((rush_read((uint8_t *)&current_data_address, REG_LASTADDR, 4) == RUSH_COMM_ERROR) && (read_attempts-- != 0));

    //protection against overflow - não sei se realmente precisa disso
    if (current_data_address <= last_data_address)
    {
        //gambiarra! com isso perdem-se alguns dados.
        last_data_address = current_data_address;
        return 0;
    }

    // Set the length of data to be read
    data_length = current_data_address - last_data_address;
    if (data_length >= RUSH_DATA_LENGTH) {
        data_length = RUSH_DATA_LENGTH;
    }

    // Read the last rush data packet
    read_attempts = COMMUNICATION_MAX_ATTEMPTS;
    while((rush_read(rush_data, last_data_address, data_length) == RUSH_COMM_ERROR) && (read_attempts-- != 0));
    last_data_address += data_length;
    return 1;
}

/**
 * \brief Check the fpga temperature to prevent damage.
 *
 * \return payload_status fpga condition
 */
uint8_t rush_overtemperature_check(void) {
    uint8_t payload_status = PAYLOAD_NOT_OK;
    int16_t fpga_temp;

    rush_read((uint8_t *)&fpga_temp, REG_FPGATEMP, 2);
    if (fpga_temp > TEMPERATURE_HIGH_LIMIT) {
        payload_status = PAYLOAD_NOT_OK;
    }
    else if (fpga_temp < TEMPERATURE_LOW_LIMIT) {
        payload_status = PAYLOAD_OK;
    }

    return payload_status;
}

void rush_delay_ms(uint16_t time_ms) {
    vTaskDelayMs(time_ms);
}

/**
 * \brief Perform a build-in health test.
 *
 * \return health_test_result status result
 */
uint8_t rush_health_test(void) {
    uint8_t tx_byte;
    uint32_t start_time;
    uint8_t health_test_result = 0;

    // Start the health test
    tx_byte = HEALTH_RUN_MASK;
    rush_write(&tx_byte, REG_HEALTH, 1);
    start_time = xTaskGetTickCount() / (uint32_t) configTICK_RATE_HZ;

    uint8_t i = 0;
    while(( xTaskGetTickCount() / (uint32_t) configTICK_RATE_HZ ) < ( start_time + HEALTH_TIMEOUT ) || i++ < HEALTH_TIMEOUT) {
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

//! \} End of rush_task group
