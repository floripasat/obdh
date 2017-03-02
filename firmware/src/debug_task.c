/*
 * debug_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include <debug_task.h>

void debug_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();

    uint8_t uart_package[512];
    uint16_t package_size;
    char cmd[10];
    char old_cmd = 0;
    request_packet_t *rqt_packet;

    while(1) {
        //TODO: TASK ROUTINE

        uart_rx(cmd, 10);
        rqt_packet = decode((uint8_t *)cmd);
        
        if(rqt_packet->request_action == SEND_DATA_REQUEST) {
            while(rqt_packet->request_data[4] > 0) {
            	package_size = get_packet(uart_package, rqt_packet->request_data);
            	if(package_size > 0) {
            		uart_tx_bytes((char *)uart_package, package_size);
            	}
            }
        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, DEBUG_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

