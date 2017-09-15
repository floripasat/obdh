/*
 * ngham_extension.h
 *
 * Copyright (C) 2014, Jon Petter Skagmo
 * Copyright (C) 2017, Gabriel Mariano Marcelino
 * 
 * This file is part of FloripaSat-TTC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>
 * 
 */

/**
 * \file ngham_externsion.h
 * 
 * \brief .
 * 
 * \author Jon Petter Skagmo <web@skagmo.com>; Mods. for FloripaSat-TTC by Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 27/04/2017
 * 
 * \defgroup ngham_extension NGHam Extension
 * \ingroup ngham
 * \{
 */

#ifndef NGHAM_EXTENSIONS_H_
#define NGHAM_EXTENSIONS_H_

#include "stdint.h"
#include "ngham_packets.h"

/**
 * \brief Possible values for the type field. After type byte, length follows.
 * 
 * \{
 */
#define PKT_TYPE_DATA           0
#define PKT_TYPE_ID             1
#define PKT_TYPE_STAT           2
#define PKT_TYPE_SIMPLEDIGI     3
#define PKT_TYPE_POS            4
#define PKT_TYPE_TOH            5
#define PKT_TYPE_DEST           6   /**< Destination/receiver callsign */
#define PKT_TYPE_CMD_REQ        7   /**< Command packet */
#define PKT_TYPE_CMD_REPLY      8   /**< Command packet */
#define PKT_TYPE_REQUEST        9
//! \}

#define PKT_TYPES               10
#define PKT_SIZE_VARIABLE       0xFFFF

/**
 * \brief Additional NA-values
 * 
 * \{
 */
#define TEMP_NA                 0xFF
#define VOLT_NA                 0xFF
#define UINT8_NA                0xFF
#define INT32_NA                0x7FFFFFFF
#define COG_NA                  0x7FF
//! \}

#define ATTRIBUTE_PACKED __attribute__ ((packed))

/**
 * \brief 
 * \{
 */
extern const char* PKT_TYPE_STRINGS[];
extern const uint16_t PKT_TYPE_SIZES[];
//! \}

/**
 * \struct ngham_toh_t
 * 
 * \brief TOH packet.
 * 
 */
typedef struct ATTRIBUTE_PACKED
{
    uint32_t toh_us;            /**< Time of hour in microseconds */
    uint8_t toh_val;            /**< Validity */
} ngham_toh_t;

/**
 * \struct ngham_stat_t
 * 
 * \brief Statistics packet.
 * 
 */
typedef struct ATTRIBUTE_PACKED
{
    uint16_t hw_ver;            /**< 10b company, 6b product */
    uint16_t serial;            /**< Serial nr. */
    uint16_t sw_ver;            /**< 4b major, 4b minor, 8b build */
    uint32_t uptime_s;          /**< Time in whole seconds since startup */
    uint8_t voltage;            /**< Input voltage in desivolts (0-25.5) */
    int8_t temp;                /**< System temp in deg. celsius (-128 to 127) */
    uint8_t signal;             /**< Received signal strength in dBm - 200, -200 to 54 (0xff=N/A) */
    uint8_t noise;              /**< Noise floor, same as above */
    uint16_t cntr_rx_ok;        /**< Packets successfully received */
    uint16_t cntr_rx_fix;       /**< Packets with corrected errors */
    uint16_t cntr_rx_err;       /**< Packets with uncorrectable errors */
    uint16_t cntr_tx;           /**< Packets sent */
} ngham_stat_t;

/**
 * \struct ngham_pos_t
 * 
 * \brief Position packet.
 * 
 */
typedef struct ATTRIBUTE_PACKED
{
    int32_t latitude;           /**< In degrees * 10^7 */
    int32_t longitude;          /**< In degrees * 10^7 */
    int32_t altitude;           /**< In centimeters */
    uint32_t sog:20;            /**< Hundreds of meters per second */
    uint16_t cog:12;            /**< Tenths of degrees */
    uint8_t hdop;               /**< In tenths */
} ngham_pos_t;

/**
 * \struct ngham_id_t
 * 
 * \brief 
 * 
 * Always first in a packet, except when resent by another station.
 * 
 */
typedef struct ATTRIBUTE_PACKED
{
    uint8_t call_ssid[6];       /**< 7 x 6 bit (SIXBIT DEC, which is ASCII-32 and limited to 0-64) empty characters padded with 0, 6 bit SSID. */
    uint8_t sequence;           /**< Wraps around from 255 to 0. */
} ngham_id_t;

/**
 * \struct ngham_dest_t
 * 
 * \brief
 * 
 */
typedef struct ATTRIBUTE_PACKED
{
    uint8_t call_ssid[6];       /**< 7 x 6 bit (SIXBIT DEC, which is ASCII-32 and limited to 0-64) empty characters padded with 0, 6 bit SSID. */
} ngham_dest_t;

/**
 * \fn ngham_ExtAllocatePkt
 * 
 * \brief 
 * 
 * Will set data type and length and increase tx_pkt length to fit data,
 * then return pointer to beginning of data without actually copying any data
 * 
 * \param *p
 * \param pkt_type
 * \param data_len
 * 
 * \return 
 */
uint8_t* ngham_ExtAllocatePkt(NGHam_TX_Packet *p, uint8_t pkt_type, uint16_t data_len);

/**
 * \fn ngham_ExtAppendPkt
 * 
 * \brief Append extension packet with given data, type and size to tx_pkt.
 * 
 * \param *p
 * \param type
 * \param *data
 * \param size
 * 
 * \return None
 */
void ngham_ExtAppendPkt(NGHam_TX_Packet *p, uint8_t type, uint8_t *data, uint16_t size);

/**
 * \fn ngham_ExtNumPkts
 * 
 * \brief 
 * 
 * \param *d
 * \param d_len
 * 
 * \return Returns number of sub packets and verifies them.
 */
uint16_t ngham_ExtNumPkts(uint8_t *d, uint16_t d_len);

/**
 * \fn ngham_ExtEncodeCallsign
 * 
 * \brief 
 * 
 * \param *enc_callsign
 * \param *callsign
 * 
 * \return 
 */
uint8_t ngham_ExtEncodeCallsign(uint8_t *enc_callsign, int8_t *callsign);

/**
 * \fn ngham_ExtDecodeCallsign
 * 
 * \brief 
 * 
 * 11 long array (7 characters, dash, two decimals and termination)
 * Destination "callsign", source "enc_callsign"
 * 
 * \param *callsign
 * \param *enc_callsign
 * 
 * \return None
 */
void ngham_ExtDecodeCallsign(int8_t *callsign, uint8_t *enc_callsign);

#endif // NGHAM_EXTENSIONS_H_

//! \} End of ngham_extension group
