/*
 * antenna.c
 *
 *  Created on: 4 de nov de 2016
 *      Author: elder
 */
#include <antenna.h>

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
