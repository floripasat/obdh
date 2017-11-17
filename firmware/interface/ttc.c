/*
 * ttc.c
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
 * \file ttc.c
 *
 * \brief Interface to deals with TT&C module
 *
 * \author Elder Tramontin
 *
 */

#include "ttc.h"

#define TIME_TO_PROCESS_CMD     1600        /**< 100 microseconds */

beacon_packet_t ttc_copy_data(void){
    beacon_packet_t beacon_packet;

    /**< V_batteries */
    beacon_packet.batteries[0]  = satellite_data.battery_monitor[4];
    beacon_packet.batteries[1]  = satellite_data.battery_monitor[5];
    beacon_packet.batteries[2]  = satellite_data.battery_monitor[6];
    beacon_packet.batteries[3]  = satellite_data.battery_monitor[7];

    /**< T_Batteries */
    beacon_packet.batteries[4] = satellite_data.ads1248[3];
    beacon_packet.batteries[5] = satellite_data.ads1248[4];
    beacon_packet.batteries[6] = satellite_data.ads1248[5];
    beacon_packet.batteries[7] = satellite_data.ads1248[6];
    beacon_packet.batteries[8] = satellite_data.ads1248[7];
    beacon_packet.batteries[9] = satellite_data.ads1248[8];

    /**< Q_Batteries */
    beacon_packet.batteries[10] = satellite_data.battery_monitor[10];
    beacon_packet.batteries[11] = satellite_data.battery_monitor[11];

    /**< I_SolarPanels */
    beacon_packet.solar_panels[0]  = satellite_data.adc_solar_panels[0];
    beacon_packet.solar_panels[1]  = satellite_data.adc_solar_panels[1];
    beacon_packet.solar_panels[2]  = satellite_data.adc_solar_panels[2];
    beacon_packet.solar_panels[3]  = satellite_data.adc_solar_panels[3];
    beacon_packet.solar_panels[4]  = satellite_data.adc_solar_panels[4];
    beacon_packet.solar_panels[5]  = satellite_data.adc_solar_panels[5];
    beacon_packet.solar_panels[6]  = satellite_data.adc_solar_panels[6];
    beacon_packet.solar_panels[7]  = satellite_data.adc_solar_panels[7];
    beacon_packet.solar_panels[8]  = satellite_data.adc_solar_panels[8];
    beacon_packet.solar_panels[9]  = satellite_data.adc_solar_panels[9];
    beacon_packet.solar_panels[10] = satellite_data.adc_solar_panels[10];
    beacon_packet.solar_panels[11] = satellite_data.adc_solar_panels[11];

    /**< V_SolarPanels */
    beacon_packet.solar_panels[12] = satellite_data.adc_solar_panels[12];
    beacon_packet.solar_panels[13] = satellite_data.adc_solar_panels[13];
    beacon_packet.solar_panels[14] = satellite_data.adc_solar_panels[14];
    beacon_packet.solar_panels[15] = satellite_data.adc_solar_panels[15];
    beacon_packet.solar_panels[16] = satellite_data.adc_solar_panels[16];
    beacon_packet.solar_panels[17] = satellite_data.adc_solar_panels[17];



    /**< IMU */
    beacon_packet.imu[0] = satellite_data.imu[0];
    beacon_packet.imu[1] = satellite_data.imu[1];
    beacon_packet.imu[2] = satellite_data.imu[2];
    beacon_packet.imu[3] = satellite_data.imu[3];
    beacon_packet.imu[4] = satellite_data.imu[4];
    beacon_packet.imu[5] = satellite_data.imu[5];
    beacon_packet.imu[6] = satellite_data.imu[6];
    beacon_packet.imu[7] = satellite_data.imu[7];
    beacon_packet.imu[8] = satellite_data.imu[8];
    beacon_packet.imu[9] = satellite_data.imu[9];
    beacon_packet.imu[10] = satellite_data.imu[10];
    beacon_packet.imu[11] = satellite_data.imu[11];

    /**< System time */
    beacon_packet.system_time[0] = satellite_data.systick[0];
    beacon_packet.system_time[1] = satellite_data.systick[1];
    beacon_packet.system_time[2] = satellite_data.systick[2];
    beacon_packet.system_time[3] = satellite_data.systick[3];

    /**< FSat status */
    beacon_packet.satellite_status[0] = satellite_data.task_scheduler[0];
    beacon_packet.satellite_status[1] = satellite_data.system_status[5];


    /**< Reset counter */
    beacon_packet.reset_counter[0] = satellite_data.system_status[0];
    beacon_packet.reset_counter[1] = satellite_data.system_status[1];

    return beacon_packet;
}

void send_data_packet() {
    FSPPacket fsp_data;
    beacon_packet_t ttc_packet;
    uint8_t ttc_pkt_data_len;
    uint8_t ttc_pkt_data[FSP_PKT_MAX_LENGTH];

    ttc_packet = ttc_copy_data();
    uint8_t ttc_packet_len = sizeof(ttc_packet);

    fsp_init(FSP_ADR_OBDH);
    fsp_gen_data_pkt((uint8_t *)&ttc_packet , ttc_packet_len, FSP_ADR_TTC, FSP_PKT_WITHOUT_ACK, &fsp_data);
    fsp_encode(&fsp_data, ttc_pkt_data, &ttc_pkt_data_len);

    sspi_tx_multiple((uint8_t*) ttc_pkt_data,
                     sizeof(ttc_pkt_data_len));       /**< send the data                                      */
}

void send_command_packet(uint8_t command) {
    FSPPacket fsp_command;
    uint8_t ttc_pkt_len, ttc_pkt_cmd_len;
    uint8_t ttc_pkt_cmd[FSP_PKT_MIN_LENGTH];

    fsp_init(FSP_ADR_OBDH);
    fsp_gen_cmd_pkt(command, FSP_ADR_TTC, FSP_PKT_WITHOUT_ACK, &fsp_command);
    fsp_encode(&fsp_command, ttc_pkt_cmd, &ttc_pkt_len);

    sspi_tx_multiple((uint8_t*) ttc_pkt_cmd,
                     sizeof(ttc_pkt_cmd_len));       /**< send the data                                      */
}

void receive_packet(uint8_t* received_packet) {
    uint8_t response[FSP_PKT_MIN_LENGTH], i, fsp_status;
    FSPPacket fsp_packet;

    sspi_rx_multiple(response, sizeof(response));

    do {
        fsp_status = fsp_decode(response[i++], &fsp_packet);
    } while(fsp_status == FSP_PKT_NOT_READY);

    if(fsp_status == FSP_PKT_READY) {
        for(i=0; i<fsp_packet.length; i++) {
            received_packet[i] = fsp_packet.payload[i];
        }
    }
}

void ttc_send_data() {
    send_command_packet(TTC_CMD_DATA_TRANSFER);     /**< send the data transfer command */
    send_data_packet();                             /**< send the data packet           */
}

uint8_t ttc_send_mutex_request() {
    uint8_t received_data;

    send_command_packet(TTC_CMD_TX_MUTEX_REQUEST);  /**< send the mutex request command */
    __delay_cycles(TIME_TO_PROCESS_CMD);            /**< wait 100us until TT&C process the received data    */
    receive_packet(&received_data);                  /**< receive the response                               */

    return received_data;
}

void ttc_tx_mutex_release() {
    send_command_packet(TTC_CMD_TX_MUTEX_RELEASE);  /**< send the mutex release command */
}

void ttc_send_shutdown() {
    uint8_t received_data;

    do {
        send_command_packet(TTC_CMD_SHUTDOWN);      /**< send the shutdown command      */
        __delay_cycles(TIME_TO_PROCESS_CMD);        /**< wait 100us until TT&C process the received data    */
        receive_packet(&received_data);              /**< receive the ACK/NACK                               */

    } while(received_data != TTC_SHUTDOWN_ACK);          /**< repeat until receive ACK                           */
}
