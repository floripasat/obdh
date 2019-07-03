/*
 * floripasat_def.h
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
 * \brief Project definitions, structures and macros
 *
 * \author Elder Tramontin
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 */

#ifndef FLORIPASATDEF_H_
#define FLORIPASATDEF_H_

#include <stdint.h>

#define SATELLITE_CALLSIGN                                  "PY0EFS"    /**< Satellite callsign. */

// Downlink packets
#define FLORIPASAT_PACKET_DOWNLINK_TELEMETRY                0x10        /**< Downlink packet with telemetry data. */
#define FLORIPASAT_PACKET_DOWNLINK_PING_ANSWER              0x11        /**< Ping answer. */
#define FLORIPASAT_PACKET_DOWNLINK_DATA_REQUEST_ANSWER      0x12        /**< Data request answer. */
#define FLORIPASAT_PACKET_DOWNLINK_HIBERNATION_FEEDBACK     0x13        /**< Hibernation feedback. */
#define FLORIPASAT_PACKET_DOWNLINK_CHARGE_RESET_FEEDBACK    0x14        /**< Charge reset feedback. */
#define FLORIPASAT_PACKET_DOWNLINK_MESSAGE_BROADCAST        0x15        /**< Message Broadcast. */
#define FLORIPASAT_PACKET_DOWNLINK_PAYLOAD_X_STATUS         0x16        /**< Payload X status. */
#define FLORIPASAT_PACKET_DOWNLINK_RUSH_FEEDBACK            0x17        /**< RUSH enable feedback. */

// Uplink packets
#define FLORIPASAT_PACKET_UPLINK_PING_REQUEST               0x20        /**< Ping request. */
#define FLORIPASAT_PACKET_UPLINK_DATA_REQUEST               0x21        /**< Data request. */
#define FLORIPASAT_PACKET_UPLINK_ENTER_HIBERNATION          0x22        /**< Enter hibernation. */
#define FLORIPASAT_PACKET_UPLINK_LEAVE_HIBERNATION          0x23        /**< Leave hibernation. */
#define FLORIPASAT_PACKET_UPLINK_CHARGE_RESET               0x24        /**< Charge reset. */
#define FLORIPASAT_PACKET_UPLINK_BROADCAST_MESSAGE          0x25        /**< Broadcast message. */
#define FLORIPASAT_PACKET_UPLINK_PAYLOAD_X_STATUS_REQUEST   0x26        /**< Payload X status request. */
#define FLORIPASAT_PACKET_UPLINK_PAYLOAD_X_SWAP             0x27        /**< Payload X swap. */
#define FLORIPASAT_PACKET_UPLINK_PAYLOAD_X_DATA_UPLOAD      0x28        /**< Payload data upload. */
#define FLORIPASAT_PACKET_UPLINK_RUSH_ENABLE                0x29        /**< RUSH enable. */

#define has_flag(x,y)   (x & y)

/**
 * \defgroup flags
 * \brief submodules flags of the data packet
 * \{
 */
#define  SYSTEM_STATUS_FLAG         BIT0
#define  IMU_FLAG                   BIT1
#define  OBDH_MISC_FLAG             BIT2
#define  SOLAR_PANELS_SENSORS_FLAG  BIT3
#define  MAIN_RADIO_FLAG            BIT4

#define  SOLAR_PANELS_FLAG          BIT5
#define  EPS_MISC_FLAG              BIT6
#define  BATTERY_MONITOR_FLAG       BIT7
#define  TEMPERATURES_FLAG          BIT8
#define  TASK_SCHEDULER_FLAG        BIT9

#define  PAYLOAD_RUSH_FLAG          BITA
#define  PAYLOAD_BRAVE_FLAG         BITB

/*
    *  This flag aware the GS to ignore the other flags, since the content
    *  of the frame is the whole data, and may disagree with the indication
    *  provided by the bit-flags.
    */
#define  WHOLE_ORBIT_DATA_FLAG      BITF

#define  ALL_FLAGS                  0xFFFF

//! \} End of flags

/**
 * \struct request_packet_t
 *
 * \brief The struct to decode a request command
 */
typedef struct {
    uint16_t flags;         /**< modules flags to request just the wanted data*/
    uint8_t packages_count; /**< number of packages to read, from offset */
    uint8_t packages_origin;/**< position used as reference for the offset */
    int32_t packages_offset;/**< number of packages to offset from origin */
} request_data_packet_t;

typedef struct {
    uint8_t id;                 /**< Packet ID code. */
    uint8_t src_callsign[7];    /**< Source callsign. */
    uint8_t data[220];          /**< Packet data. */
    uint16_t data_len;          /**< Packet data length in bytes. */
} telecommand_t;

#define PACKET_LENGTH   58  /**< according NGHAM packet sizes */

typedef struct {
    uint16_t package_flags;
    //obdh
    uint8_t obdh_uptime             [4];
    uint8_t obdh_status             [6];
    uint8_t imu                     [12];
    uint8_t obdh_misc               [6];
    uint8_t solar_panels_sensors    [12];
    uint8_t main_radio              [19];
    //eps
    uint8_t solar_panels            [18];
    uint8_t eps_misc                [8];
    uint8_t battery_monitor         [21];
    uint8_t temperatures            [21];
    uint8_t energy_level            [1];
    //payloads
    uint8_t payload_rush            [64];
    uint8_t payload_brave           [7];
} data_packet_t;



typedef struct {
    uint8_t batteries[12];          /**< bat_1_voltage[2], bat_2_voltage[2], bat_1_temp[3], bat_2_temp[3], bat_charge[2] */
    uint8_t solar_panels[18];       /**< sp_1_volt[2], sp_2_volt[2], sp_3_volt[2], sp_1_cur[2], sp_2_cur[2], sp_3_cur[2], sp_4_cur[2], sp_5_cur[2], sp_6_cur[2] */
    uint8_t satellite_status[2];    /**< energy_mode,  */
    uint8_t imu[12];                /**< accel_0_x[1], accel_0_y[1], accel_0_z[1], gyr_0_x[1], gyr_0_y[1], gyr_0_z[1], accel_1_x[1], accel_1_y[1], accel_1_z[1], gyr_1_x[1], gyr_1_y[1], gyr_1_z[1] */
    uint8_t obdh_uptime[4];         /**< time, in minutes, since deploy */
    uint8_t reset_counter[2];       /**< increment each time a reset happens */
} beacon_packet_t;

typedef struct {
    uint8_t start_of_frame;          /**< 0x7E */
    beacon_packet_t data;           /**< valid data */
    uint8_t crc_result;             /**< 8-bit crc value of the data */
} ttc_packet_t;


/**
 * \defgroup current_state_masks
 * \{
 */
#define OPERATION_MODE_MASK         0xF0    /**< to retrieve from current state register just the operation mode */
#define ENERGY_LEVEL_MASK           0x0F    /**< to retrieve from current state register just the energy level */
//! \} End of current_state_masks

/**
 * \defgroup operation_modes
 * \brief valid operation modes of the satellite
 * \{
 */
#define NORMAL_OPERATION_MODE       0x10    /**< Normal operation */
#define BOOT_MODE                   0x20    /**< While configuring the basic settings and buses */
#define HIBERNATION_MODE            0x30    /**< Hibernation (no RF transmissions) */
#define ANTENNA_DEPLOYMENT_MODE     0x40    /**< While waiting to do the deployment (45 minutes) */
//! \} End of operation_modes


/**
 * \defgroup energy_levels
 * \brief valid operation modes of the satellite
 * \{
 */
#define ENERGY_L1_MODE              0x01    /**< Energy level 1 - Tx on , Beacon 10s, TLM 60s , Payload on  */
#define ENERGY_L2_MODE              0x02    /**< Energy level 2 - Tx on , Beacon 10s, TLM 60s , Payload off */
#define ENERGY_L3_MODE              0x03    /**< Energy level 3 - Tx on , Beacon 20s, TLM 120s, Payload off */
#define ENERGY_L4_MODE              0x04    /**< Energy level 4 - Tx off, Beacon 30s, TLM off , Payload off */
//! \} End of energy_levels

/**
 * \defgroup communications
 * \brief some team definition of values used in the communications
 * \{
 */
#define CRC_SEED        0x03                    /**< The initial value of the crc operation */
#define CRC_POLYNOMIAL  0b10010010              /**< The CRC polynomial: x^7 + x^4 + x^1 */
#define START_OF_FRAME  0x7E                    /**< Initial byte of some communications: '{' */
//! \} End of communications

/**
 * \defgroup packet_origin
 * \brief used to refers to a position in the memory where are packet to read
 * \{
 */
#define OLDER_PACKAGES_ORIGIN       0   /**< refers to the older packets in the memory */
#define NEWER_PACKAGES_ORIGIN       1   /**< refers to the newer packets in the memory */
//! \} End of packet_origin

//INTERFACES
#define IMU1    0x01
#define IMU2    0x02
#define EPS     0x03
#define TTC     0x04
#define SD      0x05

//#define _DEBUG_AS_LINK  0               /**< to simulate the radio link through UART */

#define MINUTES_BEFORE_DEPLOY_ANTENNAS  45

// RUSH telecommand replies
#define RUSH_DISABLED_MSG           "RUSH experiment disabled"
#define RUSH_EN_OK_MSG              "RUSH experiment enabled for : "
#define RUSH_EN_OUT_OF_RANGE_MSG    "Out of range, RUSH experiment enabled for : 10 minutes"
#define RUSH_OUT_OF_BAT_MSG         "RUSH experiment disabled: out of battery"
#define RUSH_QUEUE_FULL_MSG         "RUSH command queue full"

#define BOOTING_MSG     "FSAT booting...\n Firmware v 0.9 - 07/07/2017\n\n"
#define CLOCK_INFO_MSG  " CLOCKS:\n  Master = 16MHz\n  Subsystem master = 16MHz\n  Auxiliary = 32768kHz \n\n"
#define CLOCK_FAIL_MSG  " *CLOCKS SETUP FAIL* \n\n"
#define UART_INFO_MSG   " UART:\n  Baudrate = 9600\n  Data bits = 8\n  Parity = None\n  Stop bits = 1\n\n"
#define I2C_INFO_MSG    " I2C:\n  Freq =~ 100kHz\n\n"
#define SPI_INF_MSG     " SPI:\n  Freq =~ 8MHz\n\n"
#define ADC_INFO_MSG    " ADC:\n  Vref+ = 3.0V\n  Vref- = GND\n\n"

volatile data_packet_t satellite_data;


#endif /* FLORIPASATDEF_H_ */

