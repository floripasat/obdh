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
 * \brief This file verify the antennas' deployment status and do the antennas' deployment sequence
 *
 * \author Elder Tramontin
 *
 */

#include "antenna.h"
#include "../include/msp_internal.h"
#include "../driver/wdti.h"
#include "../interface/wdte.h"
#include "../interface/isis_antenna.h"

#define SEQUENTIAL_DEPLOY_BURN_TIME 20      /**< max time, in seconds, while each antenna system will burn in sequential deploy */
#define OVERRIDE_DEPLOY_BURN_TIME   10      /**< time, in seconds, while each antenna system will burn in override deploy       */
#define ARMING_RETRIES              5       /**< maximum of retries to arm the antenna system                                   */

//antennas status in flash
#define ANTENNAS_NOT_DEPLOYED_IN_FLASH       0xA9
#define ANTENNAS_DEPLOYED_IN_FLASH           0x65

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
    } while(timeout++ < ARMING_RETRIES && (VERIFY_STATUS(status, ARMED) == 0) );   /**< while the deployment system is not ARMED */


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
    if( (VERIFY_STATUS(status, ANTENNAS_STATUS_MASK) == 0) ) {
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
    if( (VERIFY_STATUS(status, ANTENNAS_STATUS_MASK) == 0) ) {
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

