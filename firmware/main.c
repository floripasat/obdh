/*
 * main.c
 */
#include <obdh.h>

int main(void) {

    //vDeployAntenna();
    vWDTsConfiguration();

    vSetupHardware();

    vCreateTasks();

    vTaskStartScheduler();

    for( ;; );

	return 0; //never will reach here
}


