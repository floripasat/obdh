/*
 * main.c
 */
#include <obdh.h>

int main(void) {

    vDeployAntenna();

    vSetupHardware();

    prvCreateTasks();

    vTaskStartScheduler();

    for( ;; );

	return 0; //never will reach here
}


