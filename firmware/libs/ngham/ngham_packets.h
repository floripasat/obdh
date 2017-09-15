/*
 * ngham_packets.h
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
 * \file ngham_packets.h
 * 
 * \brief Packets.
 * 
 * \author Jon Petter Skagmo <web@skagmo.com>; Mods. for FloripaSat-TTC by Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 27/04/2017
 * 
 * \defgroup ngham_packets NGHam Packets
 * \ingroup NGHam
 * \{
 */

#ifndef NGHAM_PACKETS_H_
#define NGHAM_PACKETS_H_

#include <stdint.h>

/**
 * \brief Possible packet conditions.
 * 
 * \{
 */
#define PKT_CONDITION_OK                0   /**< Successfully received packet */
#define PKT_CONDITION_FAIL              1   /**< Receiption failed after receiving the packet */
#define PKT_CONDITION_PREFAIL           2   /**< Reception failed before receiving the whole packet */
//! \}

/**
 * \brief Packet priority in transmission.
 * 
 * \{
 */
#define PKT_PRIORITY_NORMAL             0
#define PKT_PRIORITY_FIRST_IN_SLOT      10  /**< Should be put first in the next time slot for timing purposes. */
//! \}

/**
 * \brief Size of payload in packet struct
 * 
 * \{
 */
#define PKT_PL_SIZE                     512
//! \}

/**
 * \brief Basic "not available"-values.
 * 
 * \{
 */
#define RSSI_NA                         0xFF
#define TIMESTAMP_NA                    0xFFFFFFFF
//! \}

/**
 * \brief If the following flag is set in a packet, NGHam extensions are used and first byte is type.
 * 
 * \{
 */
#define NGHAM_FLAG_TYPE_EXTENSION       0x01
//! \}

#define SPP_PL_MAX                      255 // Not a power of two, so no ring buffers!

/**
 * \brief NGHam max. lenght of the payload.
 * 
 * \{
 */
#define NGHAM_PL_MAX                    220
//! \}

#define ATTRIBUTE_PACKED __attribute__ ((packed))

/**
 * \struct rx_pkt_t
 * 
 * \brief 
 * 
 */
typedef struct ATTRIBUTE_PACKED
{
    uint32_t timestamp_toh_us;      /**< Time stamp of sync word detection */
    uint8_t noise;                  /**< Same as above */
    uint8_t rssi;                   /**< In dBm + 200 */
    uint8_t errors;                 /**< Recovered symbols */
    uint8_t ngham_flags;
    uint8_t pl[PKT_PL_SIZE];
    uint16_t pl_len;
} NGHam_RX_Packet;

/**
 * \struct tx_pkt_t
 * 
 * \brief 
 * 
 */
typedef struct ATTRIBUTE_PACKED
{
    uint8_t priority;
    uint8_t ngham_flags;
    uint8_t pl[PKT_PL_SIZE];
    uint16_t pl_len;
} NGHam_TX_Packet;

/**
 * \struct ngh_spphdr_t
 * 
 * \brief NGHam SPP header.
 * 
 */
typedef struct ATTRIBUTE_PACKED
{
    uint8_t start;
    uint16_t crc;
    uint8_t pl_type;
    uint8_t pl_len;
} ngh_spphdr_t;

/**
 * \fn ngham_RxPktInit
 * 
 * \brief RX packet initialization.
 * 
 * \param *p is a pointer to a NGHam packet.
 * 
 * \return None
 */
void ngham_RxPktInit(NGHam_RX_Packet *p);

/**
 * \fn ngham_TxPktInit
 * 
 * \brief TX packet initialization.
 * 
 * \param *p is a pointer to a NGHam packet.
 * 
 * \return None
 */
void ngham_TxPktInit(NGHam_TX_Packet *p);

/**
 * \fn ngham_TxPktGen
 * 
 * \brief Generates a TX packet with a given payload.
 * 
 * \param *p is packet to be generated.
 * \param *pl is the payload of the packet to be generated.
 * \param pl_len is the lenght of the payload.
 * 
 * \return None
 */
void ngham_TxPktGen(NGHam_TX_Packet *p, uint8_t *pl, uint8_t pl_len);

#endif // NGHAM_PACKETS_H_

//! \} End of ngham_packets group
