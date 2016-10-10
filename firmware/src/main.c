#include <msp430.h> 
#include "fsat_tasks.h"
/*
 * main.c
 */
static xTaskHandle epsTask, imuTask, ttcTask, readTemp;//, obdhTask;
static xTaskHandle debugTask;
static xTaskHandle wdtTask;

void prvCreateTasks();

int main(void) {

    vSetupHardware();

    prvCreateTasks();

    vTaskStartScheduler();

    for( ;; );

	return 0;
}

void prvCreateTasks()
{
    xTaskCreate( prvEPSTask, "EPS", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &epsTask );
    xTaskCreate( prvIMUTask, "IMU", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &imuTask );
//    xTaskCreate( prvOBDHTask, "OBDH", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &obdhTask );
    xTaskCreate( prvTTCTask, "TTC", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &ttcTask );
    xTaskCreate( prvReadTempTask, "TEMP_SENS", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &readTemp );
    xTaskCreate( prvWdtTask, "WDT", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, &wdtTask );
    xTaskCreate( prvDebugTask, "DEBUG", 4*configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &debugTask );
}
