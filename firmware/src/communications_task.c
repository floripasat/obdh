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
#ifdef _DEBUG_AS_LINK
#include "../driver/uart.h"
#endif

#define ANTENNA_MUTEX_WAIT_TIME         ( 2000 / portTICK_RATE_MS )

#define PA_ENABLE()      BIT_SET(TTC_3V3_PA_EN_OUT, TTC_3V3_PA_EN_PIN)
#define PA_DISABLE()     BIT_CLEAR(TTC_3V3_PA_EN_OUT, TTC_3V3_PA_EN_PIN)

void send_periodic_data(void);
void send_data(uint8_t *data, int16_t data_len);
uint16_t try_to_receive(uint8_t *data);
void send_requested_data(uint8_t *raw_package);
void enter_in_shutdown(void);
void request_antenna_mutex(void);
void answer_ping(telecommand_t telecommand);
void update_last_telecommand_status(telecommand_t *last_telecommand);


void communications_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    uint16_t current_turn = 0, turns_to_wait;
    uint8_t data[128];
    telecommand_t received_telecommand;
    uint8_t operation_mode;

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

        operation_mode = read_current_operation_mode();
        /**< verify if some telecommand was received on radio */
        if(try_to_receive(data) > 7) {
            received_telecommand = decode_telecommand(data);

            if(received_telecommand.request_action == REQUEST_SHUTDOWN_TELECOMMAND) {
                enter_in_shutdown();
            }

            if(operation_mode == NORMAL_OPERATION_MODE){
                if(received_telecommand.request_action == REQUEST_DATA_TELECOMMAND) {
                    send_requested_data(received_telecommand.arguments);
                }

                if(received_telecommand.request_action == REQUEST_PING_TELECOMMAND) {
    //                request_antenna_mutex();
                    answer_ping(received_telecommand);
                }
            }

            /**< update the last telecommands, rssi and counter */
            update_last_telecommand_status(&received_telecommand);
        }

        operation_mode = read_current_operation_mode();
        if(operation_mode == NORMAL_OPERATION_MODE){

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
                request_antenna_mutex();
                send_periodic_data();               /**< send the last readings of each data of the packet */

                current_turn = 0;
            }
        }

        if ( (last_wake_time + COMMUNICATIONS_TASK_PERIOD_TICKS) < xTaskGetTickCount() ) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil( (TickType_t *) &last_wake_time, COMMUNICATIONS_TASK_PERIOD_TICKS );
        }
    }

    vTaskDelete( NULL );
}

#ifdef _DEBUG_AS_LINK
void send_periodic_data(void) {
    uart_tx_bytes((char *)&satellite_data, sizeof(data_packet_t));  //send data via uart, for debug purpose
}
#else
void send_periodic_data(void) {
    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[266];
    uint16_t ngham_pkt_str_len;


   /*
    *  This flag aware the GS to ignore the other flags, since the content
    *  of the frame is the whole data, and may disagree with the indication
    *  provided by the bit-flags.
    */
    satellite_data.package_flags |= WHOLE_ORBIT_DATA_FLAG;

    ngham_TxPktGen(&ngham_packet, (uint8_t *)&satellite_data, sizeof(satellite_data));
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
    rf4463_rx_init();
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
//        rf4463_clear_interrupts();
        rx_len = rf4463_rx_packet(rx_buf, PACKET_LENGTH);  // read rx data
        rf4463_clear_interrupts();

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
    uint32_t read_position;
    uint16_t package_size = 0;
    uint8_t to_send_package[220];

    rqt_packet = decode_request_data_telecommand(raw_package);

    read_position = calculate_read_position(rqt_packet);

    if(rqt_packet.packages_count-- > 0)             /**< first packet to be sent -> send all fields of a frame */
    {
        package_size = get_packet(to_send_package, ALL_FLAGS, read_position++);
        if(package_size > 0) {
            request_antenna_mutex();
            send_data(to_send_package, package_size);
        }
    }

    while(rqt_packet.packages_count-- > 0) {
        package_size = get_packet(to_send_package, rqt_packet.flags, read_position++);
        if(package_size > 0) {
            request_antenna_mutex();
            send_data(to_send_package, package_size);
        }
    }
}

void answer_ping(telecommand_t telecommand) {
    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[220];
    uint16_t ngham_pkt_str_len;
    uint8_t answer_msg[58] = PING_MSG;
    uint8_t i;

    for(i = 0; i < 6; i++) {
        answer_msg[sizeof(PING_MSG)-1 + i] = telecommand.ID[i];
    }

    ngham_TxPktGen(&ngham_packet, answer_msg, sizeof(answer_msg));
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
    rf4463_rx_init();
}

void enter_in_shutdown(void) {
    uint8_t ttc_command;

    ttc_command = TTC_CMD_SHUTDOWN;
    xQueueOverwrite(ttc_queue, &ttc_command);   /**< send shutdown command to beacon, via ttc task      */
    xSemaphoreTake(flash_semaphore,
                   FLASH_SEMAPHORE_WAIT_TIME);  /**< protect the flash from mutual access               */
    update_operation_mode(SHUTDOWN_MODE);       /**< update the current operation mode in the flash mem */
    xSemaphoreGive(flash_semaphore);
}

void request_antenna_mutex(void) {
    uint8_t ttc_command;
    uint8_t ttc_response;

    ttc_command = TTC_CMD_TX_MUTEX_REQUEST;
    xQueueOverwrite(ttc_queue, &ttc_command);   /**< send shutdown command to beacon, via ttc task     */
    xQueueReceive(tx_queue, &ttc_response,
                  ANTENNA_MUTEX_WAIT_TIME);     /**< wait 2 seconds or until be answered               */
}

/**
 * \fn void update_last_telecommand_status(telecommand_t *last_telecommand)
 * Updates the last telecommand stored, its signal strength indicator and counter
 * \param last_telecommand Last telecommand received
 */
void update_last_telecommand_status( telecommand_t *last_telecommand ) {
    uint8_t telecommand_status[19];
    uint8_t radio_modem_status[4];
    uint8_t radio_signal_strengh;
    uint8_t i = 0;

    /**< get the radio signal strength indicator located in the last byte received */
    rf4463_get_cmd(RF4463_CMD_GET_MODEM_STATUS, radio_modem_status, 4);
    radio_signal_strengh = radio_modem_status[3];

    /**< wrap the data in a packet to be stored, via store data task */
    for(i=0; i<6; i++) {
        telecommand_status[i] = last_telecommand->ID[i];
    }
    telecommand_status[i++] = (uint8_t)last_telecommand->request_action;
    telecommand_status[i++] = (uint8_t)last_telecommand->request_action >> 8;
    for(i=8; i<16; i++) {
        telecommand_status[i] = last_telecommand->arguments[i-8];
    }
    telecommand_status[i++] = radio_signal_strengh;

    /**< send to the store data task */
    xQueueOverwrite(main_radio_queue, telecommand_status);
}
