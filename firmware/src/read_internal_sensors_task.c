/*
 * read_internal_sensors_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include <read_internal_sensors_task.h>



void read_internal_sensors_task( void *pvParameters )
{
    TickType_t last_wake_time;
    uint16_t temperature_raw, voltage_raw, current_raw;
    float temperature, voltage, current;

    last_wake_time = xTaskGetTickCount();

    obdh_setup();

    while(1)
    {
        /* read internal temperature */
        temperature_raw = obdh_temperature_read();
        temperature = obdh_temperature_convert(temperature_raw);

        /* read supply voltage */
        voltage_raw = obdh_voltage_read();
        voltage = obdh_voltage_convert(voltage_raw);

        /* read supply current */
        current_raw = obdh_current_read();
        current = obdh_current_convert(current_raw);

        //TODO: TASK ROUTINE
        sprintf(msp_internal_data, "adc value(0-4095): %u -> temp: %.3f C", temperature_raw, temperature);

        vTaskDelayUntil( (TickType_t *) &last_wake_time, READ_INTERNAL_SENSORS_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

