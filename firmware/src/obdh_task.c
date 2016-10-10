#include "obdh_task.h"

void prvOBDHTask( void *pvParameters )
{
    //static int cont = 0;

    while(1)
    {
        obdh_read(obdhData);
        //sprintf(obdhData, "OBDH DATA: %d", cont);
        //cont = (cont+1)%10;

        //F = 1Hz
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}
