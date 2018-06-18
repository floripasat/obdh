/*
 * payload_rush.h
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
 * \file payload_rush.h
 *
 * \brief Interface to deals with the payload
 *
 * \author Elder Tramontin
 *
 */

#ifndef PAYLOAD_RUSH_INTERFACE_H_
#define PAYLOAD_RUSH_INTERFACE_H_

#include "../include/floripasat_def.h"
#include "../driver/i2c.h"


#define RUSH_COMM_ERROR     0x00    /**< value when communication with payload is in error */
#define RUSH_POWER_ON       0x01    /**< value when communication with payload is "alive"  */
#define RUSH_POWER_OFF      0x02    /**< value when payload is powered off                 */
#define RUSH_FPGA_DISABLE   0x03
#define RUSH_FPGA_ENABLE    0x04

#define PAYLOAD_NOT_OK      0
#define PAYLOAD_OK          1

#define PAYLOAD_BOARD       0
#define PAYLOAD_FPGA        1
#define TURN_ON             1
#define TURN_OFF            0

#define REG_STATUS          (0x2000000)
#define REG_HEALTH          (0x2000001)
#define REG_LASTADDR        (0x2000004)
#define REG_TIME            (0x2000008)
#define REG_BUSCURRENT      (0x200000C)
#define REG_FPGATEMP        (0x2000010)
#define REG_DEBUGEN         (0x2000012)
#define REG_INIT            (0x2000013)
#define REG_HEARTBEAT       (0x2000014)

#define DEBUGEN_ENABLE_KEY  0x55
#define DEBUGEN_DISABLE_KEY 0xAA

#define STATUS_POWER_MASK           (1 << 0)
#define STATUS_CONFIGURED_MASK      (1 << 1)
#define STATUS_CONFIGSELECT_MASK    (1 << 2)
#define STATUS_SCRATCH_1_MASK       (1 << 6)
#define STATUS_SCRATCH_2_MASK       (1 << 7)

#define HEALTH_RUN_MASK             (1 << 0)
#define HEALTH_CONFIG_0_MASK        (1 << 1)
#define HEALTH_CONFIG_1_MASK        (1 << 2)
#define HEALTH_FLASH_MASK           (1 << 3)
#define HEALTH_FLASHRUN_MASK        (1 << 4)

#define HEALTH_TIMEOUT              30        /**< seconds */
#define TEMPERATURE_HIGH_LIMIT      95
#define TEMPERATURE_LOW_LIMIT       40
#define MAX_BLOCKS_PER_HEARTBEAT    3
#define BLOCK_SIZE                  32
#define RUSH_DATA_LENGTH        	( MAX_BLOCKS_PER_HEARTBEAT * BLOCK_SIZE )       /**< in bytes */

extern void rush_delay_ms( uint8_t time_ms );

/**
 * \fn rush_setup
 *
 * \brief Initialize and configure the payload
 * \return None
 */
void rush_setup( void );

/**
 * \fn rush_power_state
 *
 * \brief Turn on/off the payload board or fpga
 * \param selector chose between the board or fpga
 * \param new_power_state turn on or off
 * \return None
 */
void rush_power_state( uint8_t selector, uint8_t new_power_state );

/**
 * \fn rush_read
 *
 * \brief Read a packet of data from payload
 * \param data is a pointer where will be stored the read data
 * \param address is the payload address from the data will be read
 * \param bytes is the length, in bytes, of the read data
 * \return if the read was successful or failed
 */
uint8_t rush_read( uint8_t* data, uint32_t address, uint16_t bytes );

/**
 * \fn rush_write
 *
 * \brief Write a packet of data from payload
 * \param data is a pointer of the data to be written
 * \param address is the payload address where the data will be write
 * \param bytes is the length, in bytes, of the data to write
 * \return if the write was successful or failed
 */
uint8_t rush_write( uint8_t* data, uint32_t address, uint16_t bytes );


#endif /* PAYLOAD_RUSH_INTERFACE_H_ */
