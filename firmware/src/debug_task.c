#include <debug_task.h>

void prvDebugTask( void *pvParameters )
{
    while(1)
    {
        /* assemble the package */
        //sprintf(debugPackage, "%s\n%s\n%s\n%s\n\n", epsData, imuData, ttcData, tempSensData);

        /* send the package */
        uart_tx(epsData);
        uart_tx("\n");
        uart_tx(imuData);
        uart_tx("\n");
        uart_tx(ttcData);
        uart_tx("\n");
        uart_tx(tempSensData);
        uart_tx("\n\n");


        //F = 0.5Hz
        vTaskDelay( 500 / portTICK_PERIOD_MS );
    }
}

