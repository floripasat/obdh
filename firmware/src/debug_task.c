/*
 * debug_task.c
 *
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
 *
 * This file is part of FloripaSat-OBDH.
 *
 * FloripaSat-OBDH is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FloripaSat-OBDH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief Task that deals with the debug interface
 *
 * \author Elder Tramontin
 */

#include "debug_task.h"

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
    uint8_t operation_mode;

    while(1) {

#ifdef _DEBUG_AS_LINK
        uart_rx(cmd, sizeof(telecommand_t));

        telecommand = decode_telecommand((uint8_t*)cmd);

        if(telecommand.request_action == REQUEST_DATA_TELECOMMAND) {
            rqt_packet = decode_request_data_telecommand(telecommand.arguments);
            ttc_command = TTC_CMD_TX_MUTEX_REQUEST;
            xQueueOverwrite(ttc_queue, &ttc_command);                        //request to beacon a tx permission
            xQueueReceive(tx_queue, &ttc_response, 3000 / portTICK_RATE_MS); //wait 3 seconds or until be answered

            read_position = calculate_read_position(rqt_packet);

            while(rqt_packet.packages_count-- > 0) {

            	package_size = get_packet(uart_package, rqt_packet.flags, read_position++);
            	if(package_size > 0) {
            	    operation_mode = read_current_operation_mode();
            	    if(operation_mode == NORMAL_OPERATION_MODE){
            	        uart_tx_bytes((char *)uart_package, package_size);
            	    }
            	}
            }
            ttc_command = TTC_CMD_TX_MUTEX_RELEASE;
            xQueueOverwrite(ttc_queue, &ttc_command);
        }

        if (telecommand.request_action == REQUEST_HIBERNATION_TELECOMMAND) {
            ttc_command = TTC_CMD_HIBERNATION;
            update_operation_mode(HIBERNATION_MODE);
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
            temp_raw = (uint16_t)satellite_data.obdh_misc[0]<<8;
            temp_raw += satellite_data.obdh_misc[1];
            temperature = obdh_temperature_convert(temp_raw);
            sprintf(answer,"Temperature: %.2f C\n", temperature);
            break;
        case 'v':
            voltage_raw = (uint16_t)satellite_data.obdh_misc[2]<<8;
            voltage_raw += satellite_data.obdh_misc[3];
            voltage = obdh_voltage_convert(voltage_raw);
            sprintf(answer,"Voltage: %.2f V\n", voltage);
            break;
        case 'c':
            current_raw = (uint16_t)satellite_data.obdh_misc[4]<<8;
            current_raw += satellite_data.obdh_misc[5];
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
        if ( (last_wake_time + DEBUG_TASK_PERIOD_TICKS) < xTaskGetTickCount() ) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil( (TickType_t *) &last_wake_time, DEBUG_TASK_PERIOD_TICKS );
        }
    }

    vTaskDelete( NULL );
}

