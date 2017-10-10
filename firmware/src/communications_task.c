/*
 * communications_task.c
 *
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * \file communications_task.c
 *
 * \brief Task that deals with the downlink and uplink communications
 *
 * \author Elder Tramontin
 *
 */

#include "communications_task.h"
#if _DEBUG_AS_LINK == 1
#include "../driver/uart.h"
#endif


#define PA_ENABLE()      BIT_SET(TTC_3V3_PA_EN_OUT, TTC_3V3_PA_EN_PIN)
#define PA_DISABLE()     BIT_CLEAR(TTC_3V3_PA_EN_OUT, TTC_3V3_PA_EN_PIN)

#if _DEBUG_AS_LINK == 1
void send_periodic_data(void) {
    uart_tx_bytes((char *)&satellite_data, sizeof(data_packet_t));  //send data via uart, for debug purpouse
}
#else
void send_periodic_data(void) {
    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[266];
    uint16_t ngham_pkt_str_len;

    ngham_TxPktGen(&ngham_packet, (uint8_t *)&satellite_data, 150);
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));

    ngham_TxPktGen(&ngham_packet, (uint8_t *)&satellite_data+150, sizeof(data_packet_t)-150);
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
}
#endif



void send_data(uint8_t *data, int16_t data_len) {
    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[266];
    uint16_t ngham_pkt_str_len;

//    while(data_len > 0) { TODO: subdividir os frames em subframes
        ngham_TxPktGen(&ngham_packet, data, data_len);
        ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

        rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));

//        data_len -= 220;
//    }
}

uint16_t try_to_receive(uint8_t *data) {
    uint8_t i, rx_len;
    uint8_t rx_buf[266];
    uint16_t data_len = 0;
    uint8_t ngham_status = 0;

    if(rf4463_wait_nIRQ()) {            // verify if PACKET_RX interrupt was happened
        rf4463_clear_interrupts();
        rx_len = rf4463_rx_packet(rx_buf, PACKET_LENGTH);  // read rx data

        i = 0;
        do{
            ngham_status = ngham_Decode(rx_buf[i++], data, &data_len);
        } while(ngham_status == PKT_CONDITION_PREFAIL && i < PACKET_LENGTH && i < rx_len);

        if(ngham_status != PKT_CONDITION_OK) {
            data_len = 0;
        }

        rf4463_rx_init();    // wait for packet from tx
    }
    return data_len;
}

void send_requested_data(uint8_t *raw_package) {
    request_data_packet_t rqt_packet;
    uint8_t ttc_command  = 0;
    uint8_t ttc_response = 0;
    uint32_t read_position;
    uint16_t package_size = 0;
    uint8_t to_send_package[266];
    uint8_t operation_mode;

    operation_mode = read_current_operation_mode();
    if(operation_mode == NORMAL_OPERATION_MODE){
        rqt_packet = decode_request_data_telecommand(raw_package);
        ttc_command = TTC_CMD_TX_MUTEX_REQUEST;
        xQueueOverwrite(ttc_queue, &ttc_command);                        /**< request to beacon a tx permission */
        xQueueReceive(tx_queue, &ttc_response, 3000 / portTICK_RATE_MS); /**< wait until be answered with 3 seconds of timeout */

        read_position = calculate_read_position(rqt_packet);

        while(rqt_packet.packages_count-- > 0) {
            package_size = get_packet(to_send_package, rqt_packet.flags, read_position++);
            if(package_size > 0) {

                /**<TODO: request to beacon a tx permission */

                send_data(to_send_package, package_size);
            }
        }
    }
    ttc_command = TTC_CMD_TX_MUTEX_RELEASE;
    xQueueOverwrite(ttc_queue, &ttc_command);
}

void communications_task( void *pvParameters )
{
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    uint16_t current_turn = 0, turns_to_wait;
    uint8_t data[128];
    telecommand_t received_telecommand;
    uint8_t ttc_command;
    uint8_t energy_level;
    uint8_t radio_status = 0;

    PA_ENABLE();
    radio_status = rf4463_init();
    if(radio_status == 1) {
        rf4463_enter_standby_mode();
        ngham_Init();
        rf4463_rx_init();                   /**< start in receive mode */
    }

    while(1) {

        /**< verify if some telecommand was received on radio */
        if(try_to_receive(data) == sizeof(telecommand_t)) {
            received_telecommand = decode_telecommand(data);

            if(received_telecommand.request_action == REQUEST_DATA_TELECOMMAND) {
                send_requested_data(received_telecommand.arguments);
            }

            if(received_telecommand.request_action == REQUEST_SHUTDOWN_TELECOMMAND) {
                ttc_command = TTC_CMD_SHUTDOWN;
                update_operation_mode(SHUTDOWN_MODE);
                xQueueOverwrite(ttc_queue, &ttc_command);
            }
        }

        energy_level = read_current_energy_level();

        switch (energy_level) {
        case ENERGY_L1_MODE:
        case ENERGY_L2_MODE:
            turns_to_wait = PERIODIC_DOWNLINK_INTERVAL_TURNS;
            break;

        case ENERGY_L3_MODE:
            turns_to_wait = PERIODIC_DOWNLINK_INTERVAL_TURNS * 2;
            break;

        case ENERGY_L4_MODE:
        default:
            turns_to_wait = 0xFFFF;
        }

        if(++current_turn > turns_to_wait) {
            send_periodic_data();               /**< send the last readings of each data of the packet */
            current_turn = 0;
        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, COMMUNICATIONS_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
