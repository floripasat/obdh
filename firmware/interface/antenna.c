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

#define ALL_DEPLOYED            0

//antenna stop cause
#define TIMEOUT_CAUSE           1       /**< Value if deployment system stops because timeout                   */
#define OTHER_CAUSE             0       /**< Value if deployment system stops because other reason than timeout */

//antenna burn system
#define BURN_ACTIVE             1       /**< Value if  */
#define BURN_INACTIVE           0       /**< Value if  */


#define VERIFY_STATUS(status, bit)   ( (status & bit) > 0 )

#define ANTENNA_1               1
#define ANTENNA_2               2
#define ANTENNA_3               3
#define ANTENNA_4               4

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
    uint8_t burn_time = 10;
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
    burn_time = 20;
    start_sequential_deploy(burn_time);
    delay_s(burn_time * 4);
    status = read_deployment_status();          /**< verify what antennas are not deployed */
    if( (VERIFY_STATUS(status, ALL_ANTENNA_STATUS) == ALL_DEPLOYED) ) {
        response = ANTENNAS_DEPLOYED;
    }

    disarm_antenna();

    return response;
}

uint8_t verify_deployment_status(void) {
    uint16_t status;
    uint8_t response = ANTENNAS_NOT_DEPLOYED;

    status = read_deployment_status();
    if( (VERIFY_STATUS(status, ALL_ANTENNA_STATUS) == ALL_DEPLOYED) ) {
        response = ANTENNAS_DEPLOYED;
    }

    return response;
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
