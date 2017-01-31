/*
 * antenna.c
 *
 *  Created on: 4 de nov de 2016
 *      Author: elder
 */
#include <antenna.h>

void deploy_antenna(void){
    //TODO: I2C communication
    uint8_t deploy_status;
    /* communicates with antenna */
    deploy_status = verify_deployment_status();

    if(deploy_status == NOT_DEPLOYED) {
        //WAIT 45 MINUTES
        //TRY TO DEPLOY ANTENNA
    }

}

uint8_t verify_deployment_status(void){
    //send I2C cmd to read deployment status

}
