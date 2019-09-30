/*
 * isis_antenna.c
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
 * \brief This file deals with the ISIS antenna module
 *
 * \author Elder Tramontin
 *
 * \version 0.3.14
 *
 * \addtogroup isis_antenna
 * \{
 */

#include "isis_antenna.h"
#include "../driver/i2c.h"
#include "debug/debug.h"

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

#define DELAY_100_MS                1600000

void disarm_antenna(void) {
    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Disarming the antenna...");
    debug_new_line();

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send(ANTENNA_BASE_ADDRESS, DISARM, START_STOP);

    __delay_cycles(DELAY_100_MS);
}

void arm_antenna(void) {
    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Arming the antenna...");
    debug_new_line();

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send(ANTENNA_BASE_ADDRESS, ARM, START_STOP);

    __delay_cycles(DELAY_100_MS);
}

void start_sequential_deploy(uint8_t seconds) {
    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Executing sequential deployment...");
    debug_new_line();

    uint8_t command[2];

    command[0] = DEPLOY_SEQUENCIAL;
    command[1] = seconds;

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send_burst(ANTENNA_BASE_ADDRESS, command, 2, START_STOP);

    __delay_cycles(DELAY_100_MS);
}

void start_independet_deploy(uint8_t antenna, uint8_t seconds, uint8_t override) {
    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Executing independent deployment of antenna ");

    uint8_t command[2];

    command[1] = seconds;

    if (override == 1) {
        switch(antenna) {
            case ANTENNA_1:
                debug_print_msg("1 with override (");

                command[0] = DEPLOY_ANT_1_OVERRIDE;
                break;
            case ANTENNA_2:
                debug_print_msg("2 with override (");

                command[0] = DEPLOY_ANT_2_OVERRIDE;
                break;
            case ANTENNA_3:
                debug_print_msg("3 with override (");

                command[0] = DEPLOY_ANT_3_OVERRIDE;
                break;
            case ANTENNA_4:
                debug_print_msg("4 with override (");

                command[0] = DEPLOY_ANT_4_OVERRIDE;
                break;
            default:
                command[0] = DISARM;
        }
    }
    else {
        switch(antenna) {
            case ANTENNA_1:
                debug_print_msg("1 without override (");

                command[0] = DEPLOY_ANT_1;
                break;
            case ANTENNA_2:
                debug_print_msg("2 without override (");

                command[0] = DEPLOY_ANT_2;
                break;
            case ANTENNA_3:
                debug_print_msg("3 without override (");

                command[0] = DEPLOY_ANT_3;
                break;
            case ANTENNA_4:
                debug_print_msg("4 without override (");

                command[0] = DEPLOY_ANT_4;
                break;
            default:
                command[0] = DISARM;
        }
    }

    debug_print_dec(seconds);
    debug_print_msg(" seconds)");

    debug_new_line();

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send_burst(ANTENNA_BASE_ADDRESS, command, 2, START_STOP);

    __delay_cycles(DELAY_100_MS);
}

extern void delay_s(uint16_t seconds);

/**
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
uint16_t read_deployment_status(void) {
    uint16_t status = ANTENNAS_STATUS_MASK;               // initial state (all not deployed and disarmed

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send(ANTENNA_BASE_ADDRESS, REPORT_DEPLOY_STATUS, START_STOP);

    delay_s(1);

    i2c_set_mode(ANTENNA_BASE_ADDRESS, RECEIVE_MODE);
    if (i2c_receive_burst(ANTENNA_BASE_ADDRESS, (uint8_t *) &status, 2, START_STOP) == I2C_FAIL) {
        status = ANTENNAS_STATUS_MASK;                    // initial state (all not deployed and disarmed
    }

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);

    return status;
}

uint16_t read_antenna_temperature()
{
    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send(ANTENNA_BASE_ADDRESS, 0xB7, START_STOP);

    delay_s(1);

    i2c_set_mode(ANTENNA_BASE_ADDRESS, RECEIVE_MODE);
    uint16_t temp = 0xAAAA;
    i2c_receive_burst(ANTENNA_BASE_ADDRESS, (uint8_t *) &temp, 2, START_STOP);

    i2c_set_mode(ANTENNA_BASE_ADDRESS, TRANSMIT_MODE);

    return temp;
}

//! \} End of isis_antenna group
