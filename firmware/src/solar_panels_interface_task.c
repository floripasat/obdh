/*
 * solar_panels_interface_task.c
 *
 *  Created on: 04 de nov de 2016
 *      Author: elder
 */

#include <solar_panels_interface_task.h>

void solar_panels_interface_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    int16_t solar_panel_temperature_x, solar_panel_temperature_y, solar_panel_temperature_z;

    if (xSemaphoreTake( spi1_semaphore, portMAX_DELAY ) == pdPASS) {
        solar_panel_setup();
        xSemaphoreGive( spi1_semaphore );
    }
    while(1) {

        if (xSemaphoreTake( spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME ) == pdPASS) {
            solar_panel_temperature_x = solar_panel_read_temperature(SOLAR_PANEL_X);
            solar_panel_temperature_y = solar_panel_read_temperature(SOLAR_PANEL_Y);
            solar_panel_temperature_z = solar_panel_read_temperature(SOLAR_PANEL_Z);
            xSemaphoreGive( spi1_semaphore );
        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, SOLAR_PANELS_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
