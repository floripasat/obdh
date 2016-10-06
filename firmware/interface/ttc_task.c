#include "ttc_task.h"

void prvTTCTask( void *pvParameters )
{
    static int cont = 20;

    while(1)
    {
        sprintf(ttc_data, "TT&C DATA: %d", cont);
        cont = 20 + (cont+1)%10;
        //F = 1Hz
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}
