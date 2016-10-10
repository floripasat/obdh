#include "eps_task.h"

void prvEPSTask( void *pvParameters )
{
    static int cont = 0;

    while(1)
    {
        eps_read(epsData);
        sprintf(epsData, "EPS DATA: %d", cont);
        cont = (cont+1)%10;

        //F = 1Hz
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}
