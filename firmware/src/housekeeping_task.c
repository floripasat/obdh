/*
 * housekeeping_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../src/housekeeping_task.h"


void housekeeping_task( void *pvParameters ) {
    TickType_t last_wake_time;
    uint16_t temperature_raw, voltage_raw, current_raw;
#ifdef _DEBUG
    float temperature, voltage, current;
#endif
    uint8_t internal_sensors_data[6];
    uint8_t system_status[5];

    last_wake_time = xTaskGetTickCount();

    while(1)
    {
        /* read internal temperature */
        temperature_raw = obdh_temperature_read();

        /* read supply voltage */
        voltage_raw = obdh_voltage_read();

        /* read supply current */
        current_raw = obdh_current_read();

        internal_sensors_data[0] = temperature_raw>>8 & 0xFF;
        internal_sensors_data[1] = temperature_raw & 0xFF;
        internal_sensors_data[2] = voltage_raw>>8 & 0xFF;
        internal_sensors_data[3] = voltage_raw & 0xFF;
        internal_sensors_data[4] = current_raw>>8 & 0xFF;
        internal_sensors_data[5] = current_raw & 0xFF;

#ifdef _DEBUG

        voltage = obdh_voltage_convert(voltage_raw);
        temperature = obdh_temperature_convert(temperature_raw);
        current = obdh_current_convert(current_raw);

        satellite_data.msp_sensors[0] = internal_sensors_data[0];
        satellite_data.msp_sensors[1] = internal_sensors_data[1];
        satellite_data.msp_sensors[2] = internal_sensors_data[2];
        satellite_data.msp_sensors[3] = internal_sensors_data[3];
        satellite_data.msp_sensors[4] = internal_sensors_data[4];
        satellite_data.msp_sensors[5] = internal_sensors_data[5];
#endif

        /* Read reset counter
         * Read fault flags
         * Receive modules status (read a global variable)
         * */
        system_status[3] = read_fault_flags();

        xQueueSendToBack(system_status_queue, (void *)system_status, portMAX_DELAY);

        xQueueSendToBack(internal_sensors_queue, (void *)internal_sensors_data, portMAX_DELAY);
        vTaskDelayUntil( (TickType_t *) &last_wake_time, HOUSEKEEPING_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

