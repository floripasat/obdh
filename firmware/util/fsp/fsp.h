/*
 * fsp.h
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina.
 * 
 * This file is part of FloripaSat-FSP.
 * 
 * FloripaSat-FSP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-FSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with FloripaSat-FSP. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \file fsp.h
 * 
 * \brief FloripaSat Protocol library.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 06/11/2017
 * 
 * \defgroup fsp fsp
 * \{
 */

#ifndef FSP_H_
#define FSP_H_

#include <stdint.h>

// Packet positions
#define FSP_PKT_POS_SOD                 0
#define FSP_PKT_POS_SRC_ADR             1
#define FSP_PKT_POS_DST_ADR             2
#define FSP_PKT_POS_LEN                 3
#define FSP_PKT_POS_TYPE                4

// Start Of Data byte
#define FSP_PKT_SOD                     0x7E

// Addresses
#define FSP_ADR_EPS                     1
#define FSP_ADR_TTC                     2
#define FSP_ADR_OBDH                    3

// Types of packets
#define FSP_PKT_TYPE_DATA               1
#define FSP_PKT_TYPE_DATA_WITH_ACK      2
#define FSP_PKT_TYPE_CMD                3
#define FSP_PKT_TYPE_CMD_WITH_ACK       4
#define FSP_PKT_TYPE_ACK                5
#define FSP_PKT_TYPE_NACK               6

// Commands
#define FSP_CMD_NOP                     1
#define FSP_CMD_SEND_DATA               2
#define FSP_CMD_REQUEST_RF_MUTEX        3
#define FSP_CMD_SHUTDOWN                4

// Ack answers
#define FSP_ACK_RF_MUTEX_FREE           1
#define FSP_ACK_RF_MUTEX_BUSY           2

// Max. lengths
#define FSP_PKT_MAX_LENGTH              256
#define FSP_PAYLOAD_MAX_LENGTH          252

// CRC16 initial value (or seed byte)
#define FSP_CRC16_INITIAL_VALUE         0

// Decode states
#define FSP_PKT_NOT_READY               0
#define FSP_PKT_READY                   1
#define FSP_PKT_INVALID                 2
#define FSP_PKT_WRONG_ADR               3
#define FSP_PKT_ERROR                   4

// Config.
#define FSP_PKT_WITH_ACK                1
#define FSP_PKT_WITHOUT_ACK             0

/**
 * \struct FSPPacket
 * 
 * \brief FSP packet struct.
 */
typedef struct
{
    uint8_t sod;                                /** Start of data. */
    uint8_t src_adr;                            /** Source address. */
    uint8_t dst_adr;                            /** Destination address. */
    uint8_t length;                             /** Length of the packet payload. */
    uint8_t type;                               /** Type of packet. */
    uint8_t payload[FSP_PAYLOAD_MAX_LENGTH];    /** Payload of the packet. */
    uint16_t crc16;                             /** CRC16-CCITT bytes. */
} FSPPacket;

/**
 * \var fsp_my_adr
 * 
 * \brief The address of the module running this library.
 */
extern uint8_t fsp_my_adr;

/**
 * \var fsp_decode_pos
 * 
 * \brief Decode byte position (From the internal "state machine"),
 */
extern uint8_t fsp_decode_pos;

/**
 * \fn fsp_init
 * 
 * \brief Initializes the FSP library.
 * 
 * \param module_adr is the address of the module running this library.
 * 
 * \return None
 */
void fsp_init(uint8_t module_adr);

/**
 * \fn fsp_reset
 * 
 * \brief Resets the FSP internal state machine (The position counter) for decoding.
 * 
 * \return None
 */
void fsp_reset();

/**
 * \fn fsp_gen_data_pkt
 * 
 * \brief Generates a FSP data packet.
 * 
 * \param data is the paylod of the desired data packet.
 * \param data_len is the length of the payload of the desired data packet.
 * \param dst_adr is the destination address.
 * \param ack is the acknowledge option (FSP_PKT_WITH_ACK or FSP_PKT_WITHOUT_ACK).
 * \param fsp is a pointer to a FSPPacket struct to store the new data packet.
 * 
 * \return None
 */
void fsp_gen_data_pkt(uint8_t *data, uint8_t data_len, uint8_t dst_adr, uint8_t ack, FSPPacket *fsp);

/**
 * \fn fsp_gen_cmd_pkt
 * 
 * \brief Generates a FSP command packet.
 * 
 * \param cmd is the command of the desired command packet.
 * \param dst_adr is the destination address.
 * \param ack is the acknowledge option (FSP_PKT_WITH_ACK or FSP_PKT_WITHOUT_ACK).
 * \param fsp is a pointer to a FSPPacket struct to store the new data packet.
 * 
 * \return None
 */
void fsp_gen_cmd_pkt(uint8_t cmd, uint8_t dst_adr, uint8_t ack, FSPPacket *fsp);

/**
 * \fn fsp_gen_ack_pkt
 * 
 * \brief Generates a FSP ack. packet.
 * 
 * The ack. packet does not have a payload (The length field is zero).
 * 
 * \param dst_adr is the destination address.
 * \param fsp is a pointer to a FSPPacket struct to store the new data packet.
 * 
 * \return None
 */
void fsp_gen_ack_pkt(uint8_t dst_adr, FSPPacket *fsp);

/**
 * \fn fsp_gen_nack_pkt
 * 
 * \brief Generates a FSP nack. packet.
 * 
 * The nack. packet does not have a payload (The length field is zero).
 * 
 * \param dst_adr is the destination address.
 * \param fsp is a pointer to a FSPPacket struct to store the new data packet.
 * 
 * \return None
 */
void fsp_gen_nack_pkt(uint8_t dst_adr, FSPPacket *fsp);

/**
 * \fn fsp_gen_cmd_pkt
 * 
 * \brief Generates a generic FSP packet from a given data.
 * 
 * \param payload is the payload of the desired packet.
 * \param payload_len is the length of the payload of the desired packet.
 * \param dst_adr is the destination address.
 * \param type is the type of the packet.
 * 
 * \return None
 */
void fsp_gen_pkt(uint8_t *payload, uint8_t payload_len, uint8_t dst_adr, uint8_t type, FSPPacket *fsp);

/**
 * \fn fsp_encode
 * 
 * \brief Encodes a FSPPacket struct into a array of bytes.
 * 
 * \param fsp is a pointer to a FSPPacket struct with all the packet data.
 * \param pkt is an array to store the packet.
 * \param pkt_len is a pointer to the packet array length.
 * 
 * \return None
 */
void fsp_encode(FSPPacket *fsp, uint8_t *pkt, uint8_t *pkt_len);

/**
 * \fn fsp_decode
 * 
 * \brief Decodes a array with the FSP packet into a FSPPacket struct.
 * 
 * \param byte is the incoming byte from a FSP packet.
 * \param fsp is a pointer to a FSPPacket struct to store the packet data.
 * 
 * \return The state of the decoding process. It can be:
 *              -\b FSP_PKT_NOT_READY when all the packet data were not received yet.
 *              -\b FSP_PKT_READY when the packet is ready to be used and the decoding process is over.
 *              -\b FSP_PKT_INVALID when an invalid packet was received.
 *              -\b FSP_PKT_WRONG_ADR when the destination address of the packet is not equal to host address.
 *              -\b FSP_PKT_ERROR when an error occurs during the decoding process.
 *              .
 */
uint8_t fsp_decode(uint8_t byte, FSPPacket *fsp);

#endif // FSP_H_

//! \} End of fsp group
