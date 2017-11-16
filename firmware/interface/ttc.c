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

/* FloripaSat communication protocol */
void fsp_encode_data_packet(uint8_t *ttc_pkt_data) {

    FSPPacket fsp_data;
    beacon_packet_t ttc_packet;
    uint8_t ttc_pkt_data_len = sizeof(ttc_pkt_data);

    ttc_packet = ttc_copy_data();
    uint8_t ttc_packet_len = sizeof(ttc_packet);

    fsp_init(FSP_ADR_OBDH);
    fsp_gen_data_pkt((uint8_t *)&ttc_packet , ttc_packet_len, FSP_ADR_TTC, FSP_PKT_WITHOUT_ACK, &fsp_data);
    fsp_encode(&fsp_data, ttc_pkt_data, &ttc_pkt_data_len);
}

void fsp_encode_command_packet(uint8_t command, uint8_t *ttc_pkt_command) {

    FSPPacket fsp_command;
    uint8_t ttc_pkt_cmd_len = sizeof(ttc_pkt_command);

    fsp_init(FSP_ADR_OBDH);
    fsp_gen_cmd_pkt(command, FSP_ADR_TTC, FSP_PKT_WITHOUT_ACK, &fsp_command);
    fsp_encode(&fsp_command, ttc_pkt_command, &ttc_pkt_cmd_len);
}

void fsp_send_packet(uint8_t * ttc_packet) {
    sspi_tx_multiple((uint8_t*) ttc_packet,
                     sizeof(ttc_packet));       /**< send the data                                      */
}


void ttc_send_data(uint8_t *ttc_pkt_command, uint8_t *ttc_pkt_data) {
    fsp_encode_command_packet(TTC_CMD_DATA_TRANSFER, ttc_pkt_command);
    fsp_encode_data_packet(ttc_pkt_data);

    fsp_send_packet(ttc_pkt_command);           /**< send the data transfer command                     */
    fsp_send_packet(ttc_pkt_data);              /**< send the data packet                               */
}

uint8_t ttc_send_mutex_request(uint8_t *ttc_pkt_command) {
    uint8_t response;

    fsp_encode_command_packet(TTC_CMD_TX_MUTEX_REQUEST, ttc_pkt_command);
    fsp_send_packet(ttc_pkt_command);           /**< send the mutex request command                     */
    __delay_cycles(TIME_TO_PROCESS_CMD);        /**< wait 100us until TT&C process the received data    */
    response = sspi_rx();                       /**< receive the response                               */

    return response;
}

void ttc_tx_mutex_release(uint8_t *ttc_pkt_command) {
    fsp_encode_command_packet(TTC_CMD_TX_MUTEX_RELEASE, ttc_pkt_command);
    fsp_send_packet(ttc_pkt_command);           /**< send the mutex release command                     */
}

void ttc_send_shutdown(uint8_t *ttc_pkt_command) {
    uint8_t response;

    do {
        fsp_encode_command_packet(TTC_CMD_SHUTDOWN, ttc_pkt_command);
        fsp_send_packet(ttc_pkt_command);       /**< send the shutdown command                          */
        __delay_cycles(TIME_TO_PROCESS_CMD);    /**< wait 100us until TT&C process the received data    */
        response = sspi_rx();                   /**< receive the ACK/NACK                               */

    } while(response != TTC_SHUTDOWN_ACK);      /**< repeat until receive ACK                           */
}
