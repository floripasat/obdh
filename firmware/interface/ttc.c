/*
 * ttc.c
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
 * \brief Interface to deals with TT&C module
 *
 * \author Elder Tramontin
 */

#include "ttc.h"
#include "../include/msp_internal.h"

beacon_packet_t ttc_copy_data(void){
    beacon_packet_t beacon_packet;

    /**< V_batteries */
    beacon_packet.batteries[0]  = satellite_data.battery_monitor[4];
    beacon_packet.batteries[1]  = satellite_data.battery_monitor[5];
    beacon_packet.batteries[2]  = satellite_data.battery_monitor[6];
    beacon_packet.batteries[3]  = satellite_data.battery_monitor[7];

    /**< T_Batteries */
    beacon_packet.batteries[4] = satellite_data.temperatures[3];
    beacon_packet.batteries[5] = satellite_data.temperatures[4];
    beacon_packet.batteries[6] = satellite_data.temperatures[5];
    beacon_packet.batteries[7] = satellite_data.temperatures[6];
    beacon_packet.batteries[8] = satellite_data.temperatures[7];
    beacon_packet.batteries[9] = satellite_data.temperatures[8];

    /**< Q_Batteries */
    beacon_packet.batteries[10] = satellite_data.battery_monitor[10];
    beacon_packet.batteries[11] = satellite_data.battery_monitor[11];

    /**< I_SolarPanels */
    beacon_packet.solar_panels[0]  = satellite_data.solar_panels[0];
    beacon_packet.solar_panels[1]  = satellite_data.solar_panels[1];
    beacon_packet.solar_panels[2]  = satellite_data.solar_panels[2];
    beacon_packet.solar_panels[3]  = satellite_data.solar_panels[3];
    beacon_packet.solar_panels[4]  = satellite_data.solar_panels[4];
    beacon_packet.solar_panels[5]  = satellite_data.solar_panels[5];
    beacon_packet.solar_panels[6]  = satellite_data.solar_panels[6];
    beacon_packet.solar_panels[7]  = satellite_data.solar_panels[7];
    beacon_packet.solar_panels[8]  = satellite_data.solar_panels[8];
    beacon_packet.solar_panels[9]  = satellite_data.solar_panels[9];
    beacon_packet.solar_panels[10] = satellite_data.solar_panels[10];
    beacon_packet.solar_panels[11] = satellite_data.solar_panels[11];

    /**< V_SolarPanels */
    beacon_packet.solar_panels[12] = satellite_data.solar_panels[12];
    beacon_packet.solar_panels[13] = satellite_data.solar_panels[13];
    beacon_packet.solar_panels[14] = satellite_data.solar_panels[14];
    beacon_packet.solar_panels[15] = satellite_data.solar_panels[15];
    beacon_packet.solar_panels[16] = satellite_data.solar_panels[16];
    beacon_packet.solar_panels[17] = satellite_data.solar_panels[17];



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
    beacon_packet.obdh_uptime[0] = satellite_data.obdh_uptime[0];
    beacon_packet.obdh_uptime[1] = satellite_data.obdh_uptime[1];
    beacon_packet.obdh_uptime[2] = satellite_data.obdh_uptime[2];
    beacon_packet.obdh_uptime[3] = satellite_data.obdh_uptime[3];

    /**< FSat status */
    beacon_packet.satellite_status[0] = satellite_data.energy_level[0];
    beacon_packet.satellite_status[1] = satellite_data.obdh_status[5];


    /**< Reset counter */
    beacon_packet.reset_counter[0] = satellite_data.obdh_status[1];
    beacon_packet.reset_counter[1] = satellite_data.obdh_status[2];

    return beacon_packet;
}

void send_command_packet(uint8_t command) {
    fsp_packet_t fsp_command;
    uint8_t ttc_pkt_len;
    uint8_t ttc_pkt_cmd[FSP_PKT_MAX_LENGTH];

    fsp_init(FSP_ADR_OBDH);

    if (command == TTC_CMD_HIBERNATION)
    {
        uint8_t ttc_pkt_pl[3];

        ttc_pkt_pl[0] = FSP_CMD_HIBERNATION;
        ttc_pkt_pl[1] = (uint8_t)(get_hibernation_period_min() >> 8);
        ttc_pkt_pl[2] = (uint8_t)(get_hibernation_period_min() & 0x00FF);

        fsp_gen_pkt(ttc_pkt_pl, 3, FSP_ADR_TTC, FSP_PKT_TYPE_CMD_WITH_ACK, &fsp_command);
    }
    else
    {
        fsp_gen_cmd_pkt(command, FSP_ADR_TTC, FSP_PKT_WITH_ACK, &fsp_command);
    }

    fsp_encode(&fsp_command, ttc_pkt_cmd, &ttc_pkt_len);

    sspi_tx_multiple((uint8_t*) ttc_pkt_cmd, ttc_pkt_len);       /**< send the bytes */
}

uint8_t receive_packet(uint8_t* received_packet, uint8_t payload_len) {
    uint8_t response[FSP_PKT_MIN_LENGTH+1];
    uint8_t fsp_status = 0;
    uint8_t ack_received = TTC_NACK;
    uint8_t i = 0;
    fsp_packet_t fsp_packet;

    sspi_rx_multiple(response, FSP_PKT_MIN_LENGTH + payload_len);

    fsp_reset();
    do {
        fsp_status = fsp_decode(response[i++], &fsp_packet);
    } while(fsp_status == FSP_PKT_NOT_READY);

    if(fsp_status == FSP_PKT_READY) {
        if(fsp_packet.type == FSP_PKT_TYPE_ACK) {
            ack_received = TTC_ACK;

            for(i = 0; i < fsp_packet.length; i++) {
                received_packet[i] = fsp_packet.payload[i];
            }
        }
        if(fsp_packet.type == FSP_PKT_TYPE_NACK) {
            ack_received = TTC_NACK;
        }
    }

    return ack_received;

}

void send_data_packet(void) {
    fsp_packet_t fsp_data;
    beacon_packet_t ttc_packet;
    uint8_t ttc_pkt_data_len;
    uint8_t ttc_pkt_data[FSP_PKT_MAX_LENGTH];

    ttc_packet = ttc_copy_data();
    uint8_t ttc_packet_len = sizeof(ttc_packet);

    fsp_init(FSP_ADR_OBDH);
    fsp_gen_data_pkt((uint8_t *)&ttc_packet , ttc_packet_len, FSP_ADR_TTC, FSP_PKT_WITHOUT_ACK, &fsp_data);
    fsp_encode(&fsp_data, ttc_pkt_data, &ttc_pkt_data_len);

    sspi_tx_multiple((uint8_t*) ttc_pkt_data, ttc_pkt_data_len);       /**< send the data via SPI */
}

