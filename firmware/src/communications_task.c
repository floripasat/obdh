/*
 * communications_task.c
 *
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina
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
 * \brief Task that deals with the downlink and uplink communications
 *
 * \author Elder Tramontin
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 1.0.6
 *
 * \addtogroup communication_task
 * \{
 */

#include <stdbool.h>
#include <string.h>

#include "../interface/debug/debug.h"
#include "../config.h"

#include "communications_task.h"
#ifdef _DEBUG_AS_LINK
#include "../driver/uart.h"
#endif

//#define PAYLOAD_X

#define ANTENNA_MUTEX_WAIT_TIME         ( 2000 / portTICK_RATE_MS )

#define PA_ENABLE()                     BIT_SET(TTC_3V3_PA_EN_OUT, TTC_3V3_PA_EN_PIN)
#define PA_DISABLE()                    BIT_CLEAR(TTC_3V3_PA_EN_OUT, TTC_3V3_PA_EN_PIN)

/**
 * \brief Telecommands keys types.
 */
typedef enum
{
    KEY_ENTER_HIBERNATION = 0,      /**< Enter hibernation type. */
    KEY_LEAVE_HIBERNATION,          /**< Leave hibernation type. */
    KEY_CHARGE_RESET,               /**< Charge reset type. */
    KEY_ENABLE_RUSH                 /**< Enable RUSH type. */
} keys_e;

/**
 * \brief RUSH status.
 */
typedef enum
{
    RUSH_STATUS_READY = 0,          /**< Ready to execute an experiment. */
    RUSH_STATUS_OUT_OF_RANGE,       /**< Out of range timeout. */
    RUSH_STATUS_LOW_ENERGY,         /**< Not enough energy to execute an experiment. */
    RUSH_STATUS_BUSY                /**< Already executing an experiment. */
} rush_status_e;

void send_periodic_data(void);
void send_data(uint8_t *data, int16_t data_len);
uint16_t try_to_receive(uint8_t *data);
void send_requested_data(telecommand_t telecommand);
void enter_in_hibernation(telecommand_t telecommand);
void leave_hibernation(telecommand_t telecommand);
void request_antenna_mutex(void);
void answer_ping(telecommand_t telecommand);
void update_last_telecommand_status(telecommand_t *last_telecommand);
void send_reset_charge_command(telecommand_t telecommand);
void radioamateur_repeater(telecommand_t telecommand);
void enable_rush(telecommand_t telecommand);
bool verify_key(uint8_t *key, uint16_t key_len, uint8_t type);
void pu5gma_message(telecommand_t telecommand);

#if OBDH_PAYLOAD_X_ENABLED == 1
void send_payload_brave_data(payload_brave_downlink_t *answer);
#endif // OBDH_PAYLOAD_X_ENABLED

void unknown_telecommand(telecommand_t telecommand);

void communications_task(void *pvParameters) {
    debug_print_event_from_module(DEBUG_INFO, "Tasks", "Initializing communication task...");
    debug_new_line();

    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    uint16_t current_turn = 0, turns_to_wait;
    uint8_t data[128];
    telecommand_t received_telecommand;
    uint8_t operation_mode;

#if OBDH_PAYLOAD_X_ENABLED == 1
    payload_brave_downlink_t read_pkt;
    payload_brave_uplink_t write_pkt;
#endif // OBDH_PAYLOAD_X_ENABLED


    uint8_t energy_level;
    uint8_t radio_status = 0;
    uint8_t data_len;

    PA_ENABLE();
    radio_status = rf4463_init();
    if (radio_status == 1) {
        rf4463_enter_standby_mode();
        ngham_Init();
        rf4463_rx_init();   // start in receive mode
    }

    while(1) {
        operation_mode = read_current_operation_mode();
        // verify if some telecommand was received on radio
        data_len = try_to_receive(data);
        if (data_len >= 8) {
            received_telecommand = decode_telecommand(data, data_len);

            switch(received_telecommand.id) {
                case FLORIPASAT_PACKET_UPLINK_PING_REQUEST:
                    debug_print_event_from_module(DEBUG_INFO, "Tasks", "New \"Ping request\" telecommand received!");
                    debug_new_line();

                    if (operation_mode == NORMAL_OPERATION_MODE) {
                        answer_ping(received_telecommand);
                    }

                    break;
                case FLORIPASAT_PACKET_UPLINK_DATA_REQUEST:
                    debug_print_event_from_module(DEBUG_INFO, "Tasks", "New \"Data request\" telecommand received!");
                    debug_new_line();

                    if (operation_mode == NORMAL_OPERATION_MODE) {
                        send_requested_data(received_telecommand);
                    }

                    break;
                case FLORIPASAT_PACKET_UPLINK_ENTER_HIBERNATION:
                    debug_print_event_from_module(DEBUG_INFO, "Tasks", "New \"Enter hibernation\" telecommand received!");
                    debug_new_line();

                    enter_in_hibernation(received_telecommand);

                    break;
                case FLORIPASAT_PACKET_UPLINK_LEAVE_HIBERNATION:
                    debug_print_event_from_module(DEBUG_INFO, "Tasks", "New \"Leave hibernation\" telecommand received!");
                    debug_new_line();

                    leave_hibernation(received_telecommand);

                    break;
                case FLORIPASAT_PACKET_UPLINK_CHARGE_RESET:
                    debug_print_event_from_module(DEBUG_INFO, "Tasks", "New \"Charge reset\" telecommand received!");
                    debug_new_line();

                    send_reset_charge_command(received_telecommand);

                    break;
                case FLORIPASAT_PACKET_UPLINK_BROADCAST_MESSAGE:
                    debug_print_event_from_module(DEBUG_INFO, "Tasks", "New \"Broadcast message\" telecommand received!");
                    debug_new_line();

                    if (operation_mode == NORMAL_OPERATION_MODE) {
                        radioamateur_repeater(received_telecommand);
                    }

                    break;
                case FLORIPASAT_PACKET_UPLINK_RUSH_ENABLE:
                    debug_print_event_from_module(DEBUG_INFO, "Tasks", "New \"RUSH enable\" telecommand received!");
                    debug_new_line();

                    enable_rush(received_telecommand);

                    break;
                case 0x2F:
                    pu5gma_message(received_telecommand);

                    break;
#if OBDH_PAYLOAD_X_ENABLED == 1
                case FLORIPASAT_PACKET_UPLINK_PAYLOAD_X_TELECOMMAND:
                    write_pkt.type = PAYLOAD_BRAVE_CCSDS_TELECOMMAND;
                    memcpy(write_pkt.data.ccsds_telecommand, received_telecommand.data, sizeof(write_pkt.data.ccsds_telecommand));
                    xQueueSendToBack(payload_brave_uplink_queue, (uint8_t*)&write_pkt, portMAX_DELAY);
                    break;
                case FLORIPASAT_PACKET_UPLINK_PAYLOAD_X_DATA_UPLOAD:
                    debug_print_event_from_module(DEBUG_INFO, "Tasks", "New \"Payload-X Image\" packet received!");
                    debug_new_line();

                    write_pkt.type = PAYLOAD_BRAVE_BITSTREAM_UPLOAD;
                    memcpy(write_pkt.data.bitstream_upload, received_telecommand.data, sizeof(write_pkt.data.bitstream_upload));
                    xQueueSendToBack(payload_brave_uplink_queue, (uint8_t*)&write_pkt, portMAX_DELAY);

                    break;
                case FLORIPASAT_PACKET_UPLINK_PAYLOAD_X_SWAP:
                    debug_print_event_from_module(DEBUG_INFO, "Tasks", "New \"Payload-X swap\" telecommand received!");
                    debug_new_line();

                    write_pkt.type = PAYLOAD_BRAVE_BITSTREAM_SWAP;
                    xQueueSendToBack(payload_brave_uplink_queue, (uint8_t*)&write_pkt, portMAX_DELAY);

                    break;
                case FLORIPASAT_PACKET_UPLINK_PAYLOAD_X_STATUS_REQUEST:
                    debug_print_event_from_module(DEBUG_INFO, "Tasks", "New \"Payload-X status request\" telecommand received!");
                    debug_new_line();

                    write_pkt.type = PAYLOAD_BRAVE_BITSTREAM_STATUS_REQUEST;
                    xQueueSendToBack(payload_brave_uplink_queue, (uint8_t*)&write_pkt, portMAX_DELAY);

                    break;
#endif // OBDH_PAYLOAD_X_ENABLED
                default:
                    debug_print_event_from_module(DEBUG_WARNING, "Tasks", "Unknown telecommand received! Nothing to do!");
                    debug_new_line();

                    unknown_telecommand(received_telecommand);

                    break;
            }

            // update the last telecommands, rssi and counter
            update_last_telecommand_status(&received_telecommand);
        }

        operation_mode = read_current_operation_mode();
        if (operation_mode == NORMAL_OPERATION_MODE) {

            energy_level = read_current_energy_level();

            switch(energy_level) {
                case ENERGY_L1_MODE:
                    turns_to_wait = PERIODIC_DOWNLINK_INTERVAL_TURNS;

                    break;
                case ENERGY_L2_MODE:
                    turns_to_wait = PERIODIC_DOWNLINK_INTERVAL_TURNS;

                    break;
                case ENERGY_L3_MODE:
                    turns_to_wait = PERIODIC_DOWNLINK_INTERVAL_TURNS * 2;

                    break;
                case ENERGY_L4_MODE:
                    turns_to_wait = PERIODIC_DOWNLINK_INTERVAL_TURNS * 2;

                    break;
                case ENERGY_L5_MODE:
                    turns_to_wait = PERIODIC_DOWNLINK_INTERVAL_TURNS * 4;

                    break;
                default:
                    turns_to_wait = 0xFFFF;
            }

            if (++current_turn > turns_to_wait) {
                request_antenna_mutex();
                send_periodic_data();       // send the last readings of each data of the packet

                current_turn = 0;
            }
        }

        if ((last_wake_time + COMMUNICATIONS_TASK_PERIOD_TICKS) < xTaskGetTickCount()) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil((TickType_t *) &last_wake_time, COMMUNICATIONS_TASK_PERIOD_TICKS);
        }
#if OBDH_PAYLOAD_X_ENABLED == 1
        if (operation_mode == NORMAL_OPERATION_MODE) {
            if (xQueueReceive(payload_brave_downlink_queue, &read_pkt, 0) == pdPASS) {
                send_payload_brave_data(&read_pkt);
            }
        }
#endif // OBDH_PAYLOAD_X_ENABLED
    }

    vTaskDelete(NULL);
}

#ifdef _DEBUG_AS_LINK
void send_periodic_data(void) {
    uart_tx_bytes((char *)&satellite_data, sizeof(data_packet_t));  //send data via uart, for debug purpose
}
#else
void send_periodic_data(void) {
    if (read_current_operation_mode() == HIBERNATION_MODE) {
        return;
    }

    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[266];
    uint16_t ngham_pkt_str_len;

    uint8_t pkt_pl[220];

    // Packet ID code
    pkt_pl[0] = FLORIPASAT_PACKET_DOWNLINK_TELEMETRY;

    uint16_t i = 0;
    for(i=0; i<(7-(sizeof(SATELLITE_CALLSIGN)-1)); i++)
    {
        pkt_pl[i+1] = '0';     // Fill with 0s when the callsign length is less than 7 characters
    }

    // Source callsign
    memcpy(pkt_pl+1+i, SATELLITE_CALLSIGN, sizeof(SATELLITE_CALLSIGN)-1);

   /*
    *  This flag aware the GS to ignore the other flags, since the content
    *  of the frame is the whole data, and may disagree with the indication
    *  provided by the bit-flags.
    */
    satellite_data.package_flags |= WHOLE_ORBIT_DATA_FLAG;

    // Packet Data
    memcpy(pkt_pl+8, (uint8_t *)&satellite_data, sizeof(satellite_data) - sizeof(satellite_data.payload_brave));

    ngham_TxPktGen(&ngham_packet, pkt_pl, sizeof(satellite_data) - sizeof(satellite_data.payload_brave) + 8);
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

#if OBDH_TX_ENABLED == 1
    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
#else
    debug_print_event_from_module(DEBUG_WARNING, "Communication task", "TRANSMISSION DISABLED!");
    debug_new_line();
#endif // OBDH_TX_ENABLED

    rf4463_rx_init();
}
#endif

void send_data(uint8_t *data, int16_t data_len) {
    if (read_current_operation_mode() == HIBERNATION_MODE) {
        return;
    }

    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[266];
    uint16_t ngham_pkt_str_len;

//    while(data_len > 0) { TODO: subdividir os frames em subframes
        ngham_TxPktGen(&ngham_packet, data, data_len);
        ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

#if OBDH_TX_ENABLED == 1
        rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
#else
    debug_print_event_from_module(DEBUG_WARNING, "Communication task", "TRANSMISSION DISABLED!");
    debug_new_line();
#endif // OBDH_TX_ENABLED

//        data_len -= 220;
//    }
    rf4463_rx_init();
}

uint16_t try_to_receive(uint8_t *data) {
    uint8_t i, rx_len;
    uint8_t rx_buf[266];
    uint16_t data_len = 0;
    uint8_t ngham_status = 0;

    if (rf4463_wait_nIRQ()) {            // verify if PACKET_RX interrupt was happened
//        rf4463_clear_interrupts();
        rx_len = rf4463_rx_packet(rx_buf, PACKET_LENGTH);  // read rx data
        rf4463_clear_interrupts();

        i = 0;
        do{
            ngham_status = ngham_Decode(rx_buf[i++], data, &data_len);
        } while(ngham_status == PKT_CONDITION_PREFAIL && i < PACKET_LENGTH && i < rx_len);

        if (ngham_status != PKT_CONDITION_OK) {
            data_len = 0;
        }

        rf4463_rx_init();    // wait for packet from tx
    }

    return data_len;
}

void send_requested_data(telecommand_t telecommand) {
    if (read_current_operation_mode() == HIBERNATION_MODE) {
        return;
    }

    request_data_packet_t rqt_packet;
    uint32_t read_position;
    uint16_t package_size = 0;
    uint8_t to_send_package[220];
    uint8_t pkt_pl[160];

    // Packet ID
    pkt_pl[0] = FLORIPASAT_PACKET_DOWNLINK_DATA_REQUEST_ANSWER;

    // Source callsign
    uint16_t i = 0;
    for(i=0; i<(7-(sizeof(SATELLITE_CALLSIGN)-1)); i++) {
        pkt_pl[i+1] = '0';     // Fill with 0s when the callsign length is less than 7 characters
    }

    memcpy(pkt_pl+1+i, SATELLITE_CALLSIGN, sizeof(SATELLITE_CALLSIGN)-1);

    // Requester callsign
    for(i=0; i<7; i++) {
        pkt_pl[i+1+7] = telecommand.src_callsign[i];
    }

    // Data
    rqt_packet = decode_request_data_telecommand(telecommand.data);

    read_position = calculate_read_position(rqt_packet);

    if (rqt_packet.packages_count-- > 0) {  // first packet to be sent -> send all fields of a frame
        package_size = get_packet(to_send_package, ALL_FLAGS & (~PAYLOAD_BRAVE_FLAG), read_position++);

        for(i=0; i<package_size; i++) {
            pkt_pl[i+1+7+7] = to_send_package[i];
        }

        if (package_size > 0) {
            request_antenna_mutex();
            send_data(pkt_pl, 1+7+7+package_size);
        }

        if (rqt_packet.flags & PAYLOAD_BRAVE_FLAG)
        {
            package_size = get_packet(to_send_package, PAYLOAD_BRAVE_FLAG, read_position++);

            for(i=0; i<package_size; i++) {
                pkt_pl[i+1+7+7] = to_send_package[i];
            }

            if (package_size > 0) {
                request_antenna_mutex();
                send_data(pkt_pl, 1+7+7+package_size);
            }
        }
    }

    while(rqt_packet.packages_count-- > 0) {
        package_size = get_packet(to_send_package, rqt_packet.flags & (~PAYLOAD_BRAVE_FLAG), read_position++);

        for(i=0; i<package_size; i++) {
            pkt_pl[i+1+7+7] = to_send_package[i];
        }

        if (package_size > 0) {
            request_antenna_mutex();
            send_data(pkt_pl, 1+7+7+package_size);
        }

        if(rqt_packet.flags & PAYLOAD_BRAVE_FLAG)
        {
            package_size = get_packet(to_send_package,PAYLOAD_BRAVE_FLAG, read_position++);

            for(i=0; i<package_size; i++) {
                pkt_pl[i+1+7+7] = to_send_package[i];
            }

            if (package_size > 0) {
                request_antenna_mutex();
                send_data(pkt_pl, 1+7+7+package_size);
            }
        }
    }

    update_last_read_position(read_position);
}

void answer_ping(telecommand_t telecommand) {
    if (read_current_operation_mode() == HIBERNATION_MODE) {
        return;
    }

    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[220];
    uint16_t ngham_pkt_str_len;
    uint8_t pkt_pl[16];

    // Packet ID
    pkt_pl[0] = FLORIPASAT_PACKET_DOWNLINK_PING_ANSWER;

    uint16_t i = 0;
    for(i=0; i<(7-(sizeof(SATELLITE_CALLSIGN)-1)); i++) {
        pkt_pl[i+1] = '0';     // Fill with 0s when the callsign length is less than 7 characters
    }

    // Source callsign
    memcpy(pkt_pl+1+i, SATELLITE_CALLSIGN, sizeof(SATELLITE_CALLSIGN)-1);

    // Ping request callsign
    for(i=0; i<7; i++) {
        pkt_pl[i+8] = telecommand.src_callsign[i];
    }

    ngham_TxPktGen(&ngham_packet, pkt_pl, 15);
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

#if OBDH_TX_ENABLED == 1
    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
#else
    debug_print_event_from_module(DEBUG_WARNING, "Communication task", "TRANSMISSION DISABLED!");
    debug_new_line();
#endif // OBDH_TX_ENABLED

    rf4463_rx_init();
}

void radioamateur_repeater(telecommand_t telecommand) {
    if (read_current_operation_mode() == HIBERNATION_MODE) {
        return;
    }

    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[220];
    uint16_t ngham_pkt_str_len;
    uint8_t pkt_pl[64];

    // Packet ID
    pkt_pl[0] = FLORIPASAT_PACKET_DOWNLINK_MESSAGE_BROADCAST;

    // Source callsign
    uint16_t i = 0;
    for(i=0; i<(7-(sizeof(SATELLITE_CALLSIGN)-1)); i++) {
        pkt_pl[i+1] = '0';     // Fill with 0s when the callsign length is less than 7 characters
    }

    memcpy(pkt_pl+1+i, SATELLITE_CALLSIGN, sizeof(SATELLITE_CALLSIGN)-1);

    // Requester callsign
    for(i=0; i<7; i++) {
        pkt_pl[i+1+7] = telecommand.src_callsign[i];
    }

    // Destination callsign
    for(i=0; i<7; i++) {
        pkt_pl[i+1+7+7] = telecommand.data[i];
    }

    // Message
    uint16_t msg_len = telecommand.data_len-7;
    if (msg_len > 38)
    {
        msg_len = 38;
    }

    for(i=0; i<msg_len; i++) {
        pkt_pl[i+1+7+7+7] = telecommand.data[i+7];
    }

    ngham_TxPktGen(&ngham_packet, pkt_pl, 1+7+7+7+msg_len);
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

#if OBDH_TX_ENABLED == 1
    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
#else
    debug_print_event_from_module(DEBUG_WARNING, "Communication task", "TRANSMISSION DISABLED!");
    debug_new_line();
#endif // OBDH_TX_ENABLED

    rf4463_rx_init();
}

void enter_in_hibernation(telecommand_t telecommand) {
    // Checking it the command key is right
    uint8_t key[8];
    uint16_t key_len = telecommand.data_len-2;

    memcpy(key, telecommand.data+2, 8);

    if (!verify_key(key, key_len, KEY_ENTER_HIBERNATION))
    {
        return;     // Invalid key!
    }

    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[220];
    uint16_t ngham_pkt_str_len;
    uint8_t pkt_pl[20];

    // Packet ID
    pkt_pl[0] = FLORIPASAT_PACKET_DOWNLINK_HIBERNATION_FEEDBACK;

    // Source callsign
    uint16_t i = 0;
    for(i=0; i<(7-(sizeof(SATELLITE_CALLSIGN)-1)); i++) {
        pkt_pl[i+1] = '0';     // Fill with 0s when the callsign length is less than 7 characters
    }

    memcpy(pkt_pl+1+i, SATELLITE_CALLSIGN, sizeof(SATELLITE_CALLSIGN)-1);

    // Requester callsign
    for(i=0; i<7; i++) {
        pkt_pl[i+1+7] = telecommand.src_callsign[i];
    }

    // Hibernation duration
    pkt_pl[1+7+7]   = telecommand.data[0];
    pkt_pl[1+7+7+1] = telecommand.data[1];

    // Transmitting feedback
    ngham_TxPktGen(&ngham_packet, pkt_pl, 1+7+7+2);
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

#if OBDH_TX_ENABLED == 1
    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
#else
    debug_print_event_from_module(DEBUG_WARNING, "Communication task", "TRANSMISSION DISABLED!");
    debug_new_line();
#endif // OBDH_TX_ENABLED

    rf4463_rx_init();

    // Executing the enter hibernation command
    uint8_t ttc_command = TTC_CMD_HIBERNATION;
    xQueueOverwrite(ttc_queue, &ttc_command);                       // send shutdown command to beacon, via ttc task

    if (xSemaphoreTake(flash_semaphore, FLASH_SEMAPHORE_WAIT_TIME) == pdPASS) {     // protect the flash from mutual access
        update_operation_mode(HIBERNATION_MODE);                        // update the current operation mode in the flash mem
        set_hibernation_period_min(((uint16_t)telecommand.data[0] << 8) | telecommand.data[1]);
        xSemaphoreGive(flash_semaphore);
    }
}

void leave_hibernation(telecommand_t telecommand)
{
    // Checking it the command key is right
    uint8_t key[8];
    uint16_t key_len = telecommand.data_len;

    memcpy(key, telecommand.data, 8);

    if (!verify_key(key, key_len, KEY_LEAVE_HIBERNATION))
    {
        return;     // Invalid key!
    }

    // Executing the leave hibernation command
    if (xSemaphoreTake(flash_semaphore, FLASH_SEMAPHORE_WAIT_TIME) == pdPASS) {// protect the flash from mutual access
        update_operation_mode(NORMAL_OPERATION_MODE);                   // update the current operation mode in the flash mem
        xSemaphoreGive(flash_semaphore);
    }
}

void send_reset_charge_command(telecommand_t telecommand) {
    // Checking if the command key is right
    uint8_t key[8];
    uint16_t key_len = telecommand.data_len;

    memcpy(key, telecommand.data, 8);

    if (!verify_key(key, key_len, KEY_CHARGE_RESET)) {
        return;     // Invalid key!
    }

    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[220];
    uint16_t ngham_pkt_str_len;
    uint8_t pkt_pl[20];

    // Packet ID
    pkt_pl[0] = FLORIPASAT_PACKET_DOWNLINK_CHARGE_RESET_FEEDBACK;

    // Source callsign
    uint16_t i = 0;
    for(i=0; i<(7-(sizeof(SATELLITE_CALLSIGN)-1)); i++) {
        pkt_pl[i+1] = '0';     // Fill with 0s when the callsign length is less than 7 characters
    }

    memcpy(pkt_pl+1+i, SATELLITE_CALLSIGN, sizeof(SATELLITE_CALLSIGN)-1);

    // Requester callsign
    for(i=0; i<7; i++) {
        pkt_pl[i+1+7] = telecommand.src_callsign[i];
    }

    // Transmitting feedback
    ngham_TxPktGen(&ngham_packet, pkt_pl, 1+7+7);
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

#if OBDH_TX_ENABLED == 1
    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
#else
    debug_print_event_from_module(DEBUG_WARNING, "Communication task", "TRANSMISSION DISABLED!");
    debug_new_line();
#endif // OBDH_TX_ENABLED

    rf4463_rx_init();

    // Executing the charge reset command
    uint8_t eps_command;
    eps_command = EPS_CHARGE_RESET_CMD;
    xQueueOverwrite(eps_charge_queue, &eps_command);    // send reset charge command to eps, via eps task
}

void request_antenna_mutex(void) {
    uint8_t ttc_command;
    uint8_t ttc_response;

    ttc_command = TTC_CMD_TX_MUTEX_REQUEST;
    xQueueOverwrite(ttc_queue, &ttc_command);                           // send shutdown command to beacon, via ttc task
    xQueueReceive(tx_queue, &ttc_response, ANTENNA_MUTEX_WAIT_TIME);    // wait 2 seconds or until be answered
}

/**
 * Updates the last telecommand stored, its signal strength indicator and counter
 * \param last_telecommand Last telecommand received
 */
void update_last_telecommand_status(telecommand_t *last_telecommand) {
    uint8_t telecommand_status[220];
    uint8_t radio_modem_status[5];
    uint8_t latched_radio_signal_strengh;
    uint8_t i = 0;

    // get the radio signal strength indicator located in the last byte received
    rf4463_get_cmd(RF4463_CMD_GET_MODEM_STATUS, radio_modem_status, 5);
    latched_radio_signal_strengh = radio_modem_status[4];

    // wrap the data in a packet to be stored, via store data task
    telecommand_status[0] = last_telecommand->id;
    for(i=0; i<7; i++) {
        telecommand_status[i+1] = last_telecommand->src_callsign[i];
    }

    for(i=0; i<last_telecommand->data_len; i++) {
        telecommand_status[i+1+7] = last_telecommand->data[i];
    }

    telecommand_status[1+7+last_telecommand->data_len] = latched_radio_signal_strengh;

    // send to the store data task
    xQueueOverwrite(main_radio_queue, telecommand_status);
}

void enable_rush(telecommand_t telecommand)
{
    // Checking it the command key is right
    uint8_t key[8];
    uint16_t key_len = telecommand.data_len-1;

    memcpy(key, telecommand.data+1, 8);

    if (!verify_key(key, key_len, KEY_ENABLE_RUSH))
    {
        return;     // Invalid key!
    }

    uint8_t pkt_pl[20];

    // Packet ID
    pkt_pl[0] = FLORIPASAT_PACKET_DOWNLINK_RUSH_FEEDBACK;

    // Source callsign
    uint16_t i = 0;
    for(i=0; i<(7-(sizeof(SATELLITE_CALLSIGN)-1)); i++) {
        pkt_pl[i+1] = '0';     // Fill with 0s when the callsign length is less than 7 characters
    }

    memcpy(pkt_pl+1+i, SATELLITE_CALLSIGN, sizeof(SATELLITE_CALLSIGN)-1);

    // Requester callsign
    for(i=0; i<7; i++) {
        pkt_pl[i+1+7] = telecommand.src_callsign[i];
    }

    // Status
    pkt_pl[1+7+7] = RUSH_STATUS_READY;

    // Timeout value
    pkt_pl[1+7+7+1] = telecommand.data[0];

    // Transmitting feedback
    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[220];
    uint16_t ngham_pkt_str_len;

    ngham_TxPktGen(&ngham_packet, pkt_pl, 1+7+7+1+1);
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

#if OBDH_TX_ENABLED == 1
    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
#else
    debug_print_event_from_module(DEBUG_WARNING, "Communication task", "TRANSMISSION DISABLED!");
    debug_new_line();
#endif // OBDH_TX_ENABLED

    // Executing the telecommand
    uint8_t cmd = telecommand.data[0];

    if (read_current_energy_level() == ENERGY_L1_MODE)
    {
        if (cmd == 0)
        {
            xQueueReset(command_to_payload_rush_queue);
            xQueueSendToFront(command_to_payload_rush_queue, &cmd, 0);
            //answer = RUSH_DISABLED_MSG
        }
        else if (cmd > 30)
        {
            cmd = 10;

            if (xQueueSendToBack(command_to_payload_rush_queue, &cmd, 0) == pdTRUE)
            {
                //answer = RUSH_EN_OUT_OF_RANGE_MSG;
            }
            else
            {
                //answer = RUSH_QUEUE_FULL_MSG;
            }
        }
        else
        {
            if (xQueueSendToBack(command_to_payload_rush_queue, &cmd, 0) == pdTRUE)
            {
                //answer = RUSH_EN_OK_MSG;
                //answer[end + 1] = cmd/10 + 0x30;
                //answer[end + 2] = cmd%10 + 0x30;
            }
            else
            {
                //answer = RUSH_QUEUE_FULL_MSG;
            }
        }
    }
    else
    {
        //answer = RUSH_OUT_OF_BAT_MSG;
    }

    rf4463_rx_init();
}

bool verify_key(uint8_t *key, uint16_t key_len, uint8_t type)
{
    uint8_t key_enter_hibernation[] = OBDH_TELECOMMAND_ENTER_HIBERNATION_KEY;
    uint8_t key_leave_hibernation[] = OBDH_TELECOMMAND_LEAVE_HIBERNATION_KEY;
    uint8_t key_charge_reset[]      = OBDH_TELECOMMAND_CHARGE_RESET_KEY;
    uint8_t key_enable_rush[]       = OBDH_TELECOMMAND_ENABLE_RUSH_KEY;

    switch(type)
    {
        case KEY_ENTER_HIBERNATION:
            return memcmp(key, key_enter_hibernation, sizeof(key_enter_hibernation)-1) == 0 ? true : false;
        case KEY_LEAVE_HIBERNATION:
            return memcmp(key, key_leave_hibernation, sizeof(key_leave_hibernation)-1) == 0 ? true : false;
        case KEY_CHARGE_RESET:
            return memcmp(key, key_charge_reset, sizeof(key_charge_reset)-1) == 0 ? true : false;
        case KEY_ENABLE_RUSH:
            return memcmp(key, key_enable_rush, sizeof(key_enable_rush)-1) == 0 ? true : false;
        default:
            return false;
    }
}

#if OBDH_PAYLOAD_X_ENABLED == 1
void send_payload_brave_data(payload_brave_downlink_t *answer)
{
    if (read_current_operation_mode() == HIBERNATION_MODE)
    {
        return;
    }

    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[266];
    uint16_t ngham_pkt_str_len;

    uint8_t pkt_pl[220];



    uint16_t i = 0;
    for(i=0; i<(7-(sizeof(SATELLITE_CALLSIGN)-1)); i++)
    {
        pkt_pl[i+1] = '0';     // Fill with 0s when the callsign length is less than 7 characters
    }

    // Source callsign
    memcpy(pkt_pl+1+i, SATELLITE_CALLSIGN, sizeof(SATELLITE_CALLSIGN)-1);

   /*
    *  This flag aware the GS to ignore the other flags, since the content
    *  of the frame is the whole data, and may disagree with the indication
    *  provided by the bit-flags.
    */
    // Packet Data
    pkt_pl[8] = answer->type;

    switch (answer->type)
    {
    case PAYLOAD_BRAVE_BITSTREAM_STATUS_REPLAY:
        // Packet ID code
        pkt_pl[0] = FLORIPASAT_PACKET_DOWNLINK_PAYLOAD_X_STATUS;

        memcpy(pkt_pl+9, (void *) answer->data.bitstream_status_replay.status_segment, sizeof(answer->data.bitstream_status_replay));
        ngham_TxPktGen(&ngham_packet, pkt_pl, sizeof(answer->data.bitstream_status_replay) + 9);

        break;
    case PAYLOAD_BRAVE_CCSDS_TELEMETRY:
        // Packet ID code
        pkt_pl[0] = FLORIPASAT_PACKET_DOWNLINK_PAYLOAD_X_TELEMETRY;

        memcpy(pkt_pl+9, (void *)answer->data.ccsds_telemetry.segment, sizeof(answer->data.ccsds_telemetry));
        ngham_TxPktGen(&ngham_packet, pkt_pl, sizeof(answer->data.ccsds_telemetry) + 9);

        break;
    default:
        return;
    }

    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

#if OBDH_TX_ENABLED == 1
    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
#else
    debug_print_event_from_module(DEBUG_WARNING, "Communication task", "TRANSMISSION DISABLED!");
    debug_new_line();
#endif // OBDH_TX_ENABLED

    rf4463_rx_init();
}
#endif // OBDH_PAYLOAD_X_ENABLED

void unknown_telecommand(telecommand_t telecommand)
{
    if (read_current_operation_mode() == HIBERNATION_MODE) {
        return;
    }

    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[95];
    uint16_t ngham_pkt_str_len;
    uint8_t pkt_pl[35];

    // Packet ID
    pkt_pl[0] = 0x1F;

    uint16_t i = 0;
    for(i=0; i<(7-(sizeof(SATELLITE_CALLSIGN)-1)); i++) {
        pkt_pl[i+1] = '0';     // Fill with 0s when the callsign length is less than 7 characters
    }

    // Source callsign
    memcpy(pkt_pl+1+i, SATELLITE_CALLSIGN, sizeof(SATELLITE_CALLSIGN)-1);

    // Requester callsign
    for(i=0; i<7; i++) {
        pkt_pl[i+1+7] = telecommand.src_callsign[i];
    }

    uint8_t ays[] = "Are you sure?";
    for(i=0; sizeof(ays)-1; i++) {
        pkt_pl[i+1+7+7] = ays[i];
    }

    ngham_TxPktGen(&ngham_packet, pkt_pl, 1+7+7+sizeof(ays)-1);
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

    // Source callsign
    memcpy(pkt_pl+1+i, SATELLITE_CALLSIGN, sizeof(SATELLITE_CALLSIGN)-1);

#if OBDH_TX_ENABLED == 1
    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
#else
    debug_print_event_from_module(DEBUG_WARNING, "Communication task", "TRANSMISSION DISABLED!");
    debug_new_line();
#endif // OBDH_TX_ENABLED

    rf4463_rx_init();
}

void pu5gma_message(telecommand_t telecommand)
{
    if (read_current_operation_mode() == HIBERNATION_MODE) {
        return;
    }

    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_pkt_str[95];
    uint16_t ngham_pkt_str_len;
    uint8_t pkt_pl[35];

    // Packet ID
    pkt_pl[0] = 0x1E;

    uint16_t i = 0;
    for(i=0; i<(7-(sizeof(SATELLITE_CALLSIGN)-1)); i++) {
        pkt_pl[i+1] = '0';     // Fill with 0s when the callsign length is less than 7 characters
    }

    // Source callsign
    memcpy(pkt_pl+1+i, SATELLITE_CALLSIGN, sizeof(SATELLITE_CALLSIGN)-1);

    // Requester callsign
    for(i=0; i<7; i++) {
        pkt_pl[i+1+7] = telecommand.src_callsign[i];
    }

    uint8_t pwh[] = "PU5GMA was here";
    for(i=0; sizeof(pwh)-1; i++) {
        pkt_pl[i+1+7+7] = pwh[i];
    }

    ngham_TxPktGen(&ngham_packet, pkt_pl, 1+7+7+sizeof(pwh)-1);
    ngham_Encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

#if OBDH_TX_ENABLED == 1
    rf4463_tx_long_packet(ngham_pkt_str + (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE), ngham_pkt_str_len - (NGH_SYNC_SIZE + NGH_PREAMBLE_SIZE));
#else
    debug_print_event_from_module(DEBUG_WARNING, "Communication task", "TRANSMISSION DISABLED!");
    debug_new_line();
#endif // OBDH_TX_ENABLED

    rf4463_rx_init();
}

//! \} End of communication_task group
