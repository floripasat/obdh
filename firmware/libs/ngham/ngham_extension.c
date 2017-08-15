/*
 * ngham_extension.c
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
 * \file ngham_externsion.c
 * 
 * \brief .
 * 
 * \author Jon Petter Skagmo <web@skagmo.com>; Mods. for FloripaSat-TTC by Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 27/04/2017
 * 
 * \addtogroup ngham_extension
 * \{
 */

#include "ngham_extension.h"

#include "ngham_packets.h"
#include <stdint.h>
#include <string.h>

const char* PKT_TYPE_STRINGS[] = {
    "data",
    "id",
    "stat",
    "sdigi",
    "pos",
    "toh",
    "dest",
    "cmd_req",
    "cmd_reply",
    "request"
};

const uint16_t PKT_TYPE_SIZES[] = {
    PKT_SIZE_VARIABLE,
    sizeof(ngham_id_t),
    sizeof(ngham_stat_t),
    1,
    sizeof(ngham_pos_t),
    sizeof(ngham_toh_t),
    sizeof(ngham_dest_t),
    PKT_SIZE_VARIABLE,
    PKT_SIZE_VARIABLE,
    1
};		

uint8_t* ngham_ExtAllocatePkt(NGHam_TX_Packet *p, uint8_t pkt_type, uint16_t data_len)
{
    if ((p->pl_len + 2 + data_len) > NGHAM_PL_MAX)
    {
        return NULL;
    }
    
    p->pl[p->pl_len] = pkt_type;
    p->pl[p->pl_len+1] = data_len;
    p->pl_len += 2 + data_len;
    
    return p->pl+p->pl_len-data_len;
}

void ngh_ExtAppendPkt(NGHam_TX_Packet *p, uint8_t type, uint8_t *data, uint16_t size)
{
	if ((p->pl_len + 2 + size) > NGHAM_PL_MAX)
    {
        return;
    }
	
    p->pl[p->pl_len] = type;
	p->pl[p->pl_len+1] = size;
	memcpy(&p->pl[p->pl_len+2], data, size);
	p->pl_len += 2 + size;
}

uint16_t ngham_ExtNumPkts(uint8_t *d, uint16_t d_len)
{
	// Go through all sub packets
	uint16_t start, packets;
	start = 0;
	packets = 0;
	
	while((d_len >= (start + 2)) && (d_len >= (start + 2 + d[start + 1])))
    {
		// If PKT_TYPE is invalid valid or packet type does not have correct length
		if ((d[start]>PKT_TYPES) || ((PKT_TYPE_SIZES[d[start]] != PKT_SIZE_VARIABLE) && (PKT_TYPE_SIZES[d[start]] != d[start + 1])))
        {
			return 0;
		}
		packets++;
		start += d[start + 1] + 2; // next start
	}
    
	return packets;
}

uint8_t ngham_ExtEncodeCallsign(uint8_t *enc_callsign, int8_t *callsign)
{
	uint32_t temp;
	uint8_t j, copy[7], ssid = 0;

	// Convert to DEC SIXBIT until length is 7, zero terminated, or dash (SSID start)
	for(j=0; (j < 7) && callsign[j] && (callsign[j] != '-'); j++)
    {
		// Lowercase converted to uppercase
		if ((callsign[j] >= 0x61) && (callsign[j] <= 0x7A))
            copy[j] = callsign[j]-64;
		else
            copy[j] = callsign[j]-32;
	}
	if (j < 7)
    {
        copy[j] = 0;    // Zero terminate if preliminary end
    }
	
	// Get SSID, if any
	if (callsign[j] == '-')
    {
		j++;
		// First number
		if ((callsign[j] >= 0x30) && (callsign[j] <= 0x39))
        {
            ssid += (callsign[j] - '0');
        }
		else
        {
            return 0;
        }
		j++;
		// Second number
		if ((callsign[j] >= 0x30) && (callsign[j] <= 0x39))
        {
			ssid *= 10;
			ssid += (callsign[j] - '0');
			j++;
		}
		else if (callsign[j] != 0)
        {
            return 0;
        }
	}
	
	temp = (((uint32_t)copy[0] << 18) & 0xFC0000) | (((uint32_t)copy[1] << 12) & 0x3F000) | (((uint32_t)copy[2] << 6) & 0xFC0) | ((uint32_t)copy[3] & 0x3F);
	enc_callsign[0] = (temp >> 16) & 0xFF;
	enc_callsign[1] = (temp >> 8) & 0xFF;
	enc_callsign[2] = temp & 0xFF;

	temp = (((uint32_t)copy[4] << 18) & 0xFC0000) | (((uint32_t)copy[5] << 12) & 0x3F000) | (((uint32_t)copy[6] << 6) & 0xFC0) | ((uint32_t)ssid & 0x3F);
	enc_callsign[3] = (temp >> 16) & 0xFF;
	enc_callsign[4] = (temp >> 8) & 0xFF;
	enc_callsign[5] = temp & 0xFF;

	return 1;
}

void ngham_ExtDecodeCallsign(int8_t *callsign, uint8_t *enc_callsign)
{
    uint32_t temp;
    uint8_t j, ssid;

    temp = (((uint32_t)enc_callsign[0] << 16) & 0xFF0000) | (((uint32_t)enc_callsign[1] << 8) & 0xFF00) | ((uint32_t)enc_callsign[2] & 0xFF);
    callsign[0] = (temp >> 18) & 0x3F;
    callsign[1] = (temp >> 12) & 0x3F;
    callsign[2] = (temp >> 6) & 0x3F;
    callsign[3] = temp & 0x3F;

    temp = (((uint32_t)enc_callsign[3] << 16) & 0xFF0000) | (((uint32_t)enc_callsign[4] << 8) & 0xFF00) | ((uint32_t)enc_callsign[5] & 0xFF);
    callsign[4] = (temp >> 18) & 0x3F;
    callsign[5] = (temp >> 12) & 0x3F;
    callsign[6] = (temp >> 6) & 0x3F;
    callsign[7] = 0;	// Zero terminate (needed if max length)

    // Find end of callsign and convert from DEC SIXBIT
    for(j=0; (j < 7) && callsign[j]; j++)
    {
        callsign[j] += 32;
    }

    // If non-zero SSID
    ssid = temp & 0x3F;
    if (ssid)
    {
        callsign[j++] = '-';
        if (ssid > 9)
        {
            callsign[j++] = (ssid/10) + '0';
            ssid %= 10;
        }
        callsign[j++] = ssid + '0';
        callsign[j] = 0;
    }
}

//! \} End of ngham_extension implementation group
