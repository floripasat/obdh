#include "adc_temp_task.h"

void prvReadTempTask( void *pvParameters )
{
    float tempC;
    uint16_t adc_temp_sensor;
    float v;

    while(1)
    {
        adc_temp_sensor = adc_read();        //PUT A MUTEX HERE LATER

        v = adc_temp_sensor * (1.5/4095.0);
        tempC = (float)(((long)adc_temp_sensor - CALADC12_15V_30C) * (85.0 - 30.0)) / (CALADC12_15V_85C - CALADC12_15V_30C) + 30.0f;
        sprintf(temp_sens_data, "adc value(0-4095): %u, voltage(0-1.5): %.3fV -> temp: %.3f C", adc_temp_sensor,v, tempC);

        //F = 1Hz
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}
