#include "wdt_task.h"

void prvWdtTask( void *pvParameters )
{
    TickType_t xLastWakeTime;

    while(1)
    {
        xLastWakeTime = xTaskGetTickCount();
        //acende led
        P5OUT |= BIT4;
        vTaskDelay(10 / portTICK_PERIOD_MS );
        //apaga led
        P5OUT ^= BIT4;

        //F = 0.1Hz (100ms contados a partir da execucao de xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_PERIOD_MS );
    }
}
