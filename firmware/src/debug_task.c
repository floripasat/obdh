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
    char cmd[28];
    request_data_packet_t rqt_packet;
    telecommand_t telecommand;
    uint8_t ttc_command;
    uint8_t ttc_response;
    uint32_t read_position;

    while(1) {

#if _DEBUG_AS_LINK == 1
        uart_rx(cmd, sizeof(telecommand_t));

        telecommand = decode_telecommand(cmd);

        if(telecommand.request_action == REQUEST_DATA_TELECOMMAND) {
            rqt_packet = decode_request_data_telecommand(telecommand.arguments);
            ttc_command = TTC_CMD_REQUEST_TX;
            xQueueOverwrite(ttc_queue, &ttc_command);                        //request to beacon a tx permission
            xQueueReceive(tx_queue, &ttc_response, 3000 / portTICK_RATE_MS); //wait 3 seconds or until be answered

            read_position = calculate_read_position(rqt_packet);

            while(rqt_packet.packages_count-- > 0) {

            	package_size = get_packet(uart_package, rqt_packet.flags, read_position++);
            	if(package_size > 0) {
            		uart_tx_bytes((char *)uart_package, package_size);
            	}
            }
            ttc_command = TTC_CMD_FREE_TX;
            xQueueOverwrite(ttc_queue, &ttc_command);
        }

        if(telecommand.request_action == REQUEST_SHUTDOWN_TELECOMMAND) {
            ttc_command = TTC_CMD_SHUTDOWN;
            xQueueOverwrite(ttc_queue, &ttc_command);
        }

#else
        uart_rx(cmd, 1);
        char answer[30];
        uint16_t temp_raw, voltage_raw, current_raw;
        float temperature, voltage, current;
        uint32_t systick;
        switch(cmd[0]) {
        case 't':
            temp_raw = (uint16_t)satellite_data.msp_sensors[0]<<8;
            temp_raw += satellite_data.msp_sensors[1];
            temperature = obdh_temperature_convert(temp_raw);
            sprintf(answer,"Temperature: %.2f C\n", temperature);
            break;
        case 'v':
            voltage_raw = (uint16_t)satellite_data.msp_sensors[2]<<8;
            voltage_raw += satellite_data.msp_sensors[3];
            voltage = obdh_voltage_convert(voltage_raw);
            sprintf(answer,"Voltage: %.2f V\n", voltage);
            break;
        case 'c':
            current_raw = (uint16_t)satellite_data.msp_sensors[4]<<8;
            current_raw += satellite_data.msp_sensors[5];
            current = obdh_current_convert(current_raw);
            sprintf(answer,"Current: %.4f A\n", current);
            break;
        case 's':
            systick = xTaskGetTickCount();
            sprintf(answer,"Systick: %lu\n", systick);
            break;
        default:
            sprintf(answer,"Invalid command\n");
        }
        uart_tx(answer);

#endif

        vTaskDelayUntil( (TickType_t *) &last_wake_time, DEBUG_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

