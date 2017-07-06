/**
 * \file main.c
 *
 * \brief main file
 */
#include "obdh.h"

int main(void) {

    setup_hardware();

    if(verify_deployment_status() == ANTENNA_NOT_DEPLOYED){
        hibernate();
        antenna_deployment_routine();
    }

    create_tasks();

    vTaskStartScheduler();

    for( ;; );

	return 0; //will never reach here
}


