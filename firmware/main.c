/*
 * main.c
 */
#include <obdh.h>

int main(void) {

    setup_hardware();

    deploy_antenna();

    create_tasks();

    vTaskStartScheduler();

    for( ;; );

	return 0; //never will reach here
}


