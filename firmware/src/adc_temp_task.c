#include "adc_temp_task.h"

void prvReadTempTask( void *pvParameters )
{
    float tempC;
    uint16_t adcTempSensor;
    float v;

    while(1)
    {
        adcTempSensor = adc_read();        //PUT A MUTEX HERE LATER

        v = adcTempSensor * (1.5/4095.0);
        tempC = (float)(((long)adcTempSensor - CALADC12_15V_30C) * (85.0 - 30.0)) / (CALADC12_15V_85C - CALADC12_15V_30C) + 30.0f;
        sprintf(tempSensData, "adc value(0-4095): %u, voltage(0-1.5): %.3fV -> temp: %.3f C", adcTempSensor,v, tempC);

        //F = 1Hz
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}
