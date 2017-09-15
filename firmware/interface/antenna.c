/*
 * antenna.c
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
 * \file antenna.c
 *
 * \brief This file deals with the antenna module
 *
 * \author Elder Tramontin
 *
 */

#include "antenna.h"
#include "../include/msp_internal.h"
#include "../driver/wdti.h"
#include "../interface/wdte.h"

#define SEQUENTIAL_DEPLOY_BURN_TIME 20      /**< max time, in seconds, while each antenna system will burn in sequential deploy */
#define OVERRIDE_DEPLOY_BURN_TIME   10      /**< time, in seconds, while each antenna system will burn in override deploy       */

//MSB
#define ANTENNA_1_STATUS    BITF
#define ANTENNA_1_TIMEOUT   BITE
#define ANTENNA_1_BURNING   BITD
//NULL BIT
#define ANTENNA_2_STATUS    BITB
#define ANTENNA_2_TIMEOUT   BITA
#define ANTENNA_2_BURNING   BIT9
#define IGNORING_SWITCHES   BIT8
//LSB
#define ANTENNA_3_STATUS    BIT7
#define ANTENNA_3_TIMEOUT   BIT6
#define ANTENNA_3_BURNING   BIT5
#define INDEPENDET_BURN     BIT4
#define ANTENNA_4_STATUS    BIT3
#define ANTENNA_4_TIMEOUT   BIT2
#define ANTENNA_4_BURNING   BIT1
#define ARMED               BIT0

#define ALL_ANTENNA_STATUS  0x8888

//antenna status
#define ANTENNA_NOT_DEPLOYED    1       /**< Value if antennas are not deployed yet */
#define ANTENNA_DEPLOYED        0       /**< Value if antennas are deployed         */

//antenna stop cause
#define TIMEOUT_CAUSE           1       /**< Value if deployment system stops because timeout                   */
#define OTHER_CAUSE             0       /**< Value if deployment system stops because other reason than timeout */

//antenna burn system
#define BURN_ACTIVE             1       /**< Value if the referring antenna burn system is active */
#define BURN_INACTIVE           0       /**< Value if the referring antenna burn system is off    */

#define VERIFY_STATUS(status, bit)   ( (status & bit) > 0 )

#define ANTENNA_1               1
#define ANTENNA_2               2
#define ANTENNA_3               3
#define ANTENNA_4               4

//antennas status in flash
#define ANTENNAS_NOT_DEPLOYED_IN_FLASH       0xA9
#define ANTENNAS_DEPLOYED_IN_FLASH           0x65

// Commands
#define RESET                       0xAA
#define ARM                         0xAD
#define DISARM                      0xAC
#define DEPLOY_ANT_1                0xA1
#define DEPLOY_ANT_2                0xA2
#define DEPLOY_ANT_3                0xA3
#define DEPLOY_ANT_4                0xA4
#define DEPLOY_SEQUENCIAL           0xA5
#define DEPLOY_ANT_1_OVERRIDE       0xBA
#define DEPLOY_ANT_2_OVERRIDE       0xBB
#define DEPLOY_ANT_3_OVERRIDE       0xBC
#define DEPLOY_ANT_4_OVERRIDE       0xBD
#define DEPLOY_CANCEL               0xA9
#define MEASURE_TEMPERATURE         0xC0
#define REPORT_DEPLOY_COUNTER_1     0xB0
#define REPORT_DEPLOY_COUNTER_2     0xB1
#define REPORT_DEPLOY_COUNTER_3     0xB2
#define REPORT_DEPLOY_COUNTER_4     0xB3
#define REPORT_DEPLOY_TIMER_1       0xB4
#define REPORT_DEPLOY_TIMER_2       0xB5
#define REPORT_DEPLOY_TIMER_3       0xB6
#define REPORT_DEPLOY_TIMER_4       0xB7
#define REPORT_DEPLOY_STATUS        0xC3


void disarm_antenna(void) {

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send(ANTENNA_BASE_ADDRESS, DISARM, START_STOP);

    __delay_cycles(DELAY_100_MS_IN_CYCLES);
}

void arm_antenna(void) {

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send(ANTENNA_BASE_ADDRESS, ARM, START_STOP);

    __delay_cycles(DELAY_100_MS_IN_CYCLES);
}

void start_sequential_deploy(uint8_t seconds) {
    uint8_t command[2];

    command[0] = DEPLOY_SEQUENCIAL;
    command[1] = seconds;

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send_burst(ANTENNA_BASE_ADDRESS, command, 2, START_STOP);

    __delay_cycles(DELAY_100_MS_IN_CYCLES);
}

void start_independet_deploy(uint8_t antenna, uint8_t seconds, uint8_t override) {
    uint8_t command[2];

    command[1] = seconds;

    if(override == 1) {
        switch(antenna){
        case ANTENNA_1:
            command[0] = DEPLOY_ANT_1_OVERRIDE;
            break;
        case ANTENNA_2:
            command[0] = DEPLOY_ANT_2_OVERRIDE;
            break;
        case ANTENNA_3:
            command[0] = DEPLOY_ANT_3_OVERRIDE;
            break;
        case ANTENNA_4:
            command[0] = DEPLOY_ANT_4_OVERRIDE;
            break;
        default:
            command[0] = DISARM;
        }
    }
    else {
        switch(antenna){
        case ANTENNA_1:
            command[0] = DEPLOY_ANT_1;
            break;
        case ANTENNA_2:
            command[0] = DEPLOY_ANT_2;
            break;
        case ANTENNA_3:
            command[0] = DEPLOY_ANT_3;
            break;
        case ANTENNA_4:
            command[0] = DEPLOY_ANT_4;
            break;
        default:
            command[0] = DISARM;
        }
    }

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send_burst(ANTENNA_BASE_ADDRESS, command, 2, START_STOP);

    __delay_cycles(DELAY_100_MS_IN_CYCLES);
}

void delay_s(uint16_t seconds) {
    uint16_t i;

    for(i = 0; i < seconds; i++){
        wdte_reset_counter();
        wdti_reset_counter();
        __delay_cycles(DELAY_1_S_IN_CYCLES);
        wdte_reset_counter();
        wdti_reset_counter();
    }
}

/**
 * \fn read_deployment_status
 *
 * \brief Send a command through I2C to read the deploy switches to know if the antennas were deployed
 * \return the 16-bit antenna status, where:
 * MSB:
 *  ANTENNA_1_STATUS    BIT7
 *  ANTENNA_1_TIMEOUT   BIT6
 *  ANTENNA_1_BURNING   BIT5
 *  ANTENNA_2_STATUS    BIT3
 *  ANTENNA_2_TIMEOUT   BIT2
 *  ANTENNA_2_BURNING   BIT1
 *  IGNORING_SWITCHES   BIT0
 *
 * LSB:
 *  ANTENNA_3_STATUS    BIT7
 *  ANTENNA_3_TIMEOUT   BIT6
 *  ANTENNA_3_BURNING   BIT5
 *  INDEPENDET_BURN     BIT4
 *  ANTENNA_4_STATUS    BIT3
 *  ANTENNA_4_TIMEOUT   BIT2
 *  ANTENNA_4_BURNING   BIT1
 *  ARMED               BIT0
 */
uint16_t read_deployment_status(void){
    uint16_t status = 0x8888;               /**< initial state (all not deployed and disarmed */

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send(ANTENNA_BASE_ADDRESS, REPORT_DEPLOY_STATUS, START_STOP);

    delay_s(1);

    i2c_set_mode(ANTENNA_BASE_ADDRESS, RECEIVE_MODE);
    if( i2c_receive_burst(ANTENNA_BASE_ADDRESS, (uint8_t *) &status, 2, START_STOP) == I2C_FAIL){
        status = 0x8888;                    /**< initial state (all not deployed and disarmed */
    }
    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);

    return status;
}

uint8_t antennas_deployment_routine(void){

    uint16_t status;
    uint8_t response = ANTENNAS_NOT_DEPLOYED;
    uint8_t burn_time = OVERRIDE_DEPLOY_BURN_TIME;
    uint8_t timeout = 0;

    /**< try to arm antenna */
    do {
        arm_antenna();
        delay_s(1);
        status = read_deployment_status();
    } while(timeout++ < 5 && (VERIFY_STATUS(status, ARMED) == 0) );   /**< while the deployment system is not ARMED */


    /**< send a command to do the override deploy */
    start_independet_deploy(ANTENNA_1, burn_time, 1);   /**< try to override the closed antennas */
    delay_s(burn_time);
    start_independet_deploy(ANTENNA_2, burn_time, 1);   /**< try to override the closed antennas */
    delay_s(burn_time);
    start_independet_deploy(ANTENNA_3, burn_time, 1);   /**< try to override the closed antennas */
    delay_s(burn_time);
    start_independet_deploy(ANTENNA_4, burn_time, 1);   /**< try to override the closed antennas */
    delay_s(burn_time);


    /*
     * try to deploy sequential, 20 seconds of burn
     */
    burn_time = SEQUENTIAL_DEPLOY_BURN_TIME;
    start_sequential_deploy(burn_time);
    delay_s(burn_time * 4);
    status = read_deployment_status();          /**< verify what antennas are not deployed */
    if( (VERIFY_STATUS(status, ALL_ANTENNA_STATUS) == 0) ) {
        response = ANTENNAS_DEPLOYED;
    }

    disarm_antenna();

    return response;
}

uint8_t read_antenna_status_in_memory(void) {
    uint8_t status[3];
    uint8_t response = ANTENNAS_NOT_DEPLOYED_IN_FLASH;

    status[0] = flash_read_single(ANTENNA_STATUS_ADDR_FLASH);
    status[1] = flash_read_single(ANTENNA_STATUS_ADDR_FLASH + 1);
    status[2] = flash_read_single(ANTENNA_STATUS_ADDR_FLASH + 2);


    /**< Consensus algorithm */
    if( (status[0] == status[1]) ||
        (status[0] == status[2])   ) {  /**< status[0] has the same value than [1] or [2] */
        response = status[0];
    }
    else {                              /**< status[0] has a different value than [1] and [2] */
        if(status[1] == status[2]) {    /**< the [1] and [2] values are equals                */
            response = status[1];
        }
        else {                          /**< the 3 values are different                       */
            response = ANTENNAS_NOT_DEPLOYED_IN_FLASH;
        }
    }
    return response;
}

void save_deployed_status_in_memory(void) {
    flash_erase((uint32_t *) ANTENNA_STATUS_ADDR_FLASH);
    flash_write_single(ANTENNAS_DEPLOYED_IN_FLASH, ANTENNA_STATUS_ADDR_FLASH);
    flash_write_single(ANTENNAS_DEPLOYED_IN_FLASH, ANTENNA_STATUS_ADDR_FLASH+1);
    flash_write_single(ANTENNAS_DEPLOYED_IN_FLASH, ANTENNA_STATUS_ADDR_FLASH+2);
}


uint8_t verify_deployment_status(void) {
    uint16_t status;
    uint8_t response = ANTENNAS_NOT_DEPLOYED;

    status = read_deployment_status();
    if( (VERIFY_STATUS(status, ALL_ANTENNA_STATUS) == 0) ) {
        response = ANTENNAS_DEPLOYED;
    }

    if(read_antenna_status_in_memory() == ANTENNAS_DEPLOYED_IN_FLASH) {
        response = ANTENNAS_DEPLOYED;
    }
    else {
        if(response == ANTENNAS_DEPLOYED) {
            save_deployed_status_in_memory();
        }
    }

    return response;
}

