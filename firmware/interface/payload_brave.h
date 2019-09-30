/*
 * payload_brave.h
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
 * \file payload_brave.h
 *
 * \brief Interface to deals with the Payload 1
 *
 * \author Elder Tramontin
 *
 */

#ifndef PAYLOAD_BRAVE_INTERFACE_H_
#define PAYLOAD_BRAVE_INTERFACE_H_

#include "../include/floripasat_def.h"
#include "../driver/i2c.h"
#include "../util/crc.h"

#define PAYLOAD_BRAVE_TIMEOUT_ERROR       0x00
#define PAYLOAD_BRAVE_OK                  0x01
#define PAYLOAD_BRAVE_CRC_ERROR           0x02
#define PAYLOAD_BRAVE_ACK                 0x03
#define PAYLOAD_BRAVE_NACK                0x04
#define PAYLOAD_BRAVE_INVALID_COMMAND     0x05
#define PAYLOAD_BRAVE_NO_DATA             0x06

#define PAYLOAD_BRAVE_COMM_ERROR     0x00    /**< value when communication with payload is in error */
#define PAYLOAD_BRAVE_POWER_ON       0x01    /**< value when communication with payload is "alive"  */
#define PAYLOAD_BRAVE_POWER_OFF      0x02    /**< value when payload is powered off                 */

#define PAYLOAD_BRAVE_CCSDS_TELECOMMAND          'T'    /**< command to request telecommand to playloadx*/
#define PAYLOAD_BRAVE_BITSTREAM_UPLOAD           'U'    /**< command to request playloadx bitstream upload*/
#define PAYLOAD_BRAVE_BITSTREAM_SWAP             'V'    /**< command to request a swap version of payloadx bitstream*/
#define PAYLOAD_BRAVE_BITSTREAM_STATUS_REQUEST   'S'    /**< command to request the status of bitstream frames*/
#define PAYLOAD_BRAVE_BITSTREAM_STATUS_REPLAY    'S'
#define PAYLOAD_BRAVE_CCSDS_TELEMETRY            'T'
#define PAYLOAD_BRAVE_NO_PENDING_DATA            'N'


#define PAYLOAD_BRAVE_BITSTEAM_STATUS_REPLAY_LENGHT      220+7
#define PAYLOAD_BRAVE_CCSDE_TELEMETRY_LENGHT             193+7
#define PAYLOAD_BRAVE_BITSTREAM_SWAP_LENGHT              1
#define PAYLOAD_BRAVE_BITSTREAM_UPLOAD_LENGHT            84
#define PAYLOAD_BRAVE_CCSDS_TELECOMMAND_LENGHT           82
#define PAYLOAD_BRAVE_BITSTREAM_STATUS_REQUEST_LENGHT    1


uint8_t payload_brave_read();

uint8_t payload_brave_write();


static inline void payload_brave_power_on()
{
    BIT_SET(P5OUT, BIT2);
}

static inline void payload_brave_power_down()
{
    BIT_CLEAR(P5OUT, BIT2);
}

static inline void payload_brave_setup()
{
    payload_brave_power_on();
    BIT_SET(P5DIR, BIT2);
}
#endif /* PAYLOAD_BRAVE_INTERFACE_H_ */
