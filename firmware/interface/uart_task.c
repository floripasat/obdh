#include "uart_task.h"

void prvSendUartTask( void *pvParameters )
{
    static char uart_package[UART_PACKAGE_LENGTH];

    while(1)
    {
        /* assemble the package */
        sprintf(uart_package, "%s\n%s\n%s\n%s\n\n", eps_data, imu_data, ttc_data, temp_sens_data);

        /* send the package */
        uart_tx(uart_package);

        //F = 0.5Hz
        vTaskDelay( 500 / portTICK_PERIOD_MS );
    }
}
