#include <msp430.h> 
#include "fsat_tasks.h"
/*
 * main.c
 */
int main(void) {

    vSetupHardware();
    //vTraceInitTraceData();


   /* if(uiTraceStart() == 0)
    {
        while(1); //ERROR -> an error message is found in RecorderDataPtr->systemInfo.
    }*/
    static xTaskHandle epsTask, imuTask, ttcTask, readTemp;
    static xTaskHandle uartSend;
    static xTaskHandle wdtTask;


    xTaskCreate( prvEPSTask, "EPS", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &epsTask );
    xTaskCreate( prvIMUTask, "IMU", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &imuTask );
    xTaskCreate( prvTTCTask, "TTC", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &ttcTask );
    xTaskCreate( prvReadTempTask, "TEMP_SENS", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &readTemp );
    xTaskCreate( prvWdtTask, "WDT", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, &wdtTask );
    xTaskCreate( prvSendUartTask, "UartSend", 4*configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &uartSend );

    vTaskStartScheduler();

    for( ;; );

	return 0;
}
