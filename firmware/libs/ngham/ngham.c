/*
 * ngham.c
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
 * \file ngham.c
 * 
 * \brief NGHam protocol functions.
 * 
 * \author Jon Petter Skagmo <web@skagmo.com>; Mods. for FloripaSat-TTC by Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \date 10/02/2017
 *
 * \addtogroup ngham
 * \{
 */

#include <stddef.h>                     // For NULL etc.

#include "ngham.h"
#include "ccsds_scrambler.h"            // Pre-generated array from scrambling polynomial
#include "ngham_packets.h"              // Structs for TX and RX packets
#include "crc_ccitt.h"
#include "platform/platform.h"

#include <stdio.h>

// There are seven different sizes.
// Each size has a correlation tag for size, a total size, a maximum payload size and a parity data size.
const uint8_t NGH_PL_SIZE[]         = {28,   60,   92,   124,  156,  188,  220};    // Actual payload
const uint8_t NGH_PL_SIZE_FULL[]    = {31,   63,   95,   127,  159,  191,  223};    // Size with LEN, payload and CRC
const uint8_t NGH_PL_PAR_SIZE[]     = {47,   79,   111,  159,  191,  223,  255};    // Size with RS parity added
const uint8_t NGH_PAR_SIZE[]        = {16,   16,   16,   32,   32,   32,   32};

// The seven different size tag vectors
const uint32_t NGH_SIZE_TAG[] = {
    0b001110110100100111001101,
    0b010011011101101001010111,
    0b011101101001001110011010,
    0b100110111011010010101110,
    0b101000001111110101100011,
    0b110101100110111011111001,
    0b111011010010011100110100
};

// Preamble and synchronization vector
const uint8_t NGH_PREAMBLE              = 0xAA;
const uint8_t NGH_SYNC[]                = {0x5D, 0xE6, 0x2A, 0x7E};
const uint8_t NGH_PREAMBLE_FOUR_LEVEL   = 0xDD;
const uint8_t NGH_SYNC_FOUR_LEVEL[]     = {0x77, 0xf7, 0xfd, 0x7d, 0x5d, 0xdd, 0x7f, 0xfd};

// Reed Solomon control blocks for the different NGHAM sizes
RS rs_cb[NGH_SIZES];

void ngham_Init()
{
    decoder_state = NGH_STATE_SIZE_TAG;
    
    ngham_InitArrays();
}

void ngham_InitArrays()
{
    uint8_t i;
    for(i=0;i<NGH_SIZES;i++)
    {
        rs_cb[i].mm         = 0;
        rs_cb[i].nn         = 0;
        rs_cb[i].alpha_to   = (uint8_t*)0;
        rs_cb[i].index_of   = (uint8_t*)0;
        rs_cb[i].genpoly    = (uint8_t*)0;
        rs_cb[i].nroots     = 0;
        rs_cb[i].fcr        = 0;
        rs_cb[i].prim       = 0;
        rs_cb[i].iprim      = 0;
        rs_cb[i].pad        = NGH_PL_PAR_SIZE[6] - NGH_PL_PAR_SIZE[i];
    }
}

void ngham_DeinitArrays()
{
    free_rs_char(&rs_cb[0]);    // Free memory for nroots = 16
    free_rs_char(&rs_cb[3]);    // Free memory for nroots = 32
}

/**
 * \fn ngham_TagCheck
 * 
 * \brief Check hamming distance.
 * 
 * Used to check if hamming distance in size tag is smaller than treshold.
 * 
 * NGHam library internal function.
 * 
 * \param x
 * \param y
 * 
 * \return It can return:
 *          -\b NGH_HAMMING_DISTANCE_GREATER if false
 *          -\b NGH_HAMMING_DISTANCE_SMALLER if true
 *          .
 */
static uint8_t ngham_TagCheck(uint32_t x, uint32_t y)
{
    uint8_t j, distance;
    uint32_t diff;
    diff = x^y;
    if (!diff)
    {
        return NGH_HAMMING_DISTANCE_SMALLER;	// Early check to save time
    }

    distance = 0;
    for(j=0; j<24; j++)
    {
        if (diff & 0x01)
        {
            distance++;
            if (distance > NGH_SIZE_TAG_MAX_ERROR)
            {
                return NGH_HAMMING_DISTANCE_GREATER;
            }
        }
        diff >>= 0x01;
    }
    
    return NGH_HAMMING_DISTANCE_SMALLER;
}

void ngham_Encode(NGHam_TX_Packet *p, uint8_t *pkt, uint16_t *pkt_len)
{
    uint16_t j;
    uint16_t crc;
    uint8_t size_nr = 0;
    uint8_t d[NGH_MAX_TOT_SIZE];
    uint16_t d_len = 0;
    uint8_t codeword_start;

    // Check size and find control block for smallest possible RS codeword
    if ((p->pl_len == 0) || (p->pl_len > NGH_PL_SIZE[NGH_SIZES-1]))
    {
        return;
    }
    while(p->pl_len > NGH_PL_SIZE[size_nr])
    {
        size_nr++;
    }

    // Insert preamble, sync and size-tag
    if (NGHAM_FOUR_LEVEL_MODULATION)
    {
        codeword_start = NGH_PREAMBLE_SIZE_FOUR_LEVEL + NGH_SYNC_SIZE_FOUR_LEVEL + NGH_SIZE_TAG_SIZE;
        for(j=0; j<NGH_PREAMBLE_SIZE_FOUR_LEVEL; j++)
        {
            d[d_len++] = NGH_PREAMBLE_FOUR_LEVEL;
        }
        for(j=0; j<NGH_SYNC_SIZE_FOUR_LEVEL; j++)
        {
            d[d_len++] = NGH_SYNC_FOUR_LEVEL[j];
        }
    }
    else
    {
        codeword_start = NGH_PREAMBLE_SIZE + NGH_SYNC_SIZE + NGH_SIZE_TAG_SIZE;
        for(j=0; j<NGH_PREAMBLE_SIZE; j++)
        {
            d[d_len++] = NGH_PREAMBLE;
        }
        for(j=0; j<NGH_SYNC_SIZE; j++)
        {
            d[d_len++] = NGH_SYNC[j];
        }
    }
    d[d_len++] = (NGH_SIZE_TAG[size_nr] >> 16) & 0xFF;
    d[d_len++] = (NGH_SIZE_TAG[size_nr] >> 8) & 0xFF;
    d[d_len++] = NGH_SIZE_TAG[size_nr] & 0xFF;

    // Prepare content of codeword
    d[d_len] = (NGH_PL_SIZE[size_nr] - p->pl_len) & 0x1F;	// Insert padding size
    d[d_len] |= (p->ngham_flags << 5) & 0xE0;				// Insert flags
    d_len++;
    for(j=0; j<p->pl_len; j++)
    {
        d[d_len++] = p->pl[j];		// Insert data
    }
    crc = ngham_CRC_CCITT(&d[codeword_start], p->pl_len+1);	// Insert CRC
    d[d_len++] = (crc >> 8) & 0xFF;
    d[d_len++] = crc & 0xFF;
    for(j=p->pl_len+3; j<NGH_PL_SIZE_FULL[size_nr]; j++)
    {
        d[d_len++] = 0;	// Insert padding
    }

    // Generate parity data
    encode_rs_char(&rs_cb[size_nr], &d[codeword_start], &d[d_len]);
    d_len += NGH_PAR_SIZE[size_nr];

    // Scramble
    for(j=0; j<NGH_PL_PAR_SIZE[size_nr]; j++)
    {
        d[codeword_start+j] ^= ccsds_poly[j];
    }

    ngham_ActionSendData(d, d_len, p->priority, pkt, pkt_len);
}

uint8_t ngham_Decode(uint8_t d, uint8_t *msg, uint16_t *msg_len)
{
    static uint8_t size_nr;
    static uint32_t size_tag;
    static uint16_t length;
    // This points to the address one lower than the payload!
    static uint8_t *buf = (uint8_t*)&rx_pkt.ngham_flags;

    switch(decoder_state)
    {
        case NGH_STATE_SIZE_TAG:
            size_tag = 0;
            ngham_ActionReceptionStarted();
            
        case NGH_STATE_SIZE_TAG_2:
            size_tag <<= 8;
            size_tag |= d;
            decoder_state++;
            break;

        case NGH_STATE_SIZE_TAG_3:
            size_tag <<= 8;
            size_tag |= d;
            {
                for (size_nr=0; size_nr<NGH_SIZES; size_nr++)
                {
                    // If tag is intact, set known size
                    if (ngham_TagCheck(size_tag, NGH_SIZE_TAG[size_nr]))
                    {
                        decoder_state = NGH_STATE_SIZE_KNOWN;
                        length = 0;

                        // Set new packet size as soon as possible
                        ngham_ActionSetPacketSize(NGH_PL_PAR_SIZE[size_nr] + NGH_SIZE_TAG_SIZE);
                        break;
                    }
                }
                // If size tag is not found, every size can theoretically be attempted
                if (decoder_state != NGH_STATE_SIZE_KNOWN)
                {
                    ngham_ActionHandlePacket(PKT_CONDITION_PREFAIL, NULL, NULL, NULL);
                    decoder_state = NGH_STATE_SIZE_TAG;
                }
            }
            break;

        case NGH_STATE_SIZE_KNOWN:
            // De-scramble byte and append to buffer
            buf[length] = d^ccsds_poly[length];
            length++;

            // Do whatever is necessary in this action
            if (length == NGHAM_BYTES_TILL_ACTION_HALFWAY)
            {
                ngham_ActionReceptionHalfway();
            }

            if (length == NGH_PL_PAR_SIZE[size_nr])
            {
                int8_t errors;

                // Set packet size back to a large value
                ngham_ActionSetPacketSize(255);
                decoder_state = NGH_STATE_SIZE_TAG;

                // Run Reed Solomon decoding, calculate packet length
                //errors = decode_rs_char(&rs_cb[size_nr], buf, 0, 0);
                errors = 0; //TODO: REMOVE THAT IN FUTURE
                rx_pkt.pl_len = NGH_PL_SIZE[size_nr] - (buf[0] & NGH_PADDING_bm);

                // Check if the packet is decodeable and then if CRC is OK
                if ((errors != -1) && (ngham_CRC_CCITT(buf, rx_pkt.pl_len+1) == ((buf[rx_pkt.pl_len+1]<<8) | buf[rx_pkt.pl_len+2])) )
                {

                    // Copy remaining fields and pass on
                    rx_pkt.errors = errors;
                    rx_pkt.ngham_flags = (buf[0] & NGH_FLAGS_bm) >> NGH_FLAGS_bp;
                    rx_pkt.noise = ngham_ActionGetNoiseFloor();
                    rx_pkt.rssi = ngham_ActionGetRSSI();
                    ngham_ActionHandlePacket(PKT_CONDITION_OK, &rx_pkt, msg, msg_len);
                    return PKT_CONDITION_OK;
                }
                // If packet decoding not was successful, count this as an error
                else
                {
                    ngham_ActionHandlePacket(PKT_CONDITION_FAIL, NULL, NULL, NULL);
                    return PKT_CONDITION_FAIL;
                }
            }
            break;
    }

    return PKT_CONDITION_PREFAIL;
}

//! \} End of NGHam implementation group
