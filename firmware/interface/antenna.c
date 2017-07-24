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

void deploy_antenna(void){
    //TODO: include the I2C code to send the deployment command
}

void antenna_deployment_routine(void){

    do {
        deploy_antenna();
    } while(verify_deployment_status() == ANTENNA_NOT_DEPLOYED);

}


uint8_t verify_deployment_status(void){
    //send I2C cmd to read deployment status
    return ANTENNA_DEPLOYED;   //while the routine was not implemented TODO: implements the antenna communication
}
