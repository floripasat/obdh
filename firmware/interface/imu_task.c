#include "imu_task.h"


void prvIMUTask( void *pvParameters )
{
    static int cont = 10;

    while(1)
    {
        sprintf(imu_data, "IMU DATA: %d", cont++);
        cont = 10 + (cont+1)%10;
        //F = 1Hz
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}
