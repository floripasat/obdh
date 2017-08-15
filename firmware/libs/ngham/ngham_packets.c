/*
 * ngham_packets.c
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
 * \brief .
 * 
 * \author Jon Petter Skagmo <web@skagmo.com>; Mods. for FloripaSat-TTC by Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 27/04/2017
 * 
 * \addtogroup ngham_packets
 * \{
 */

#include "ngham_packets.h"

void ngham_RxPktInit(NGHam_RX_Packet *p)
{
    p->pl_len           = 0;
    p->ngham_flags      = 0;
    p->rssi             = RSSI_NA;
    p->noise            = RSSI_NA;
    p->errors           = 0;
    p->timestamp_toh_us = TIMESTAMP_NA;
}

void ngham_TxPktInit(NGHam_TX_Packet *p)
{
    p->pl_len       = 0;
    p->ngham_flags  = 0;
    p->priority     = PKT_PRIORITY_NORMAL;
}

void ngham_TxPktGen(NGHam_TX_Packet *p, uint8_t *pl, uint8_t pl_len)
{
    p->pl_len         = pl_len;
    p->ngham_flags    = 0;
    p->priority       = PKT_PRIORITY_NORMAL;
    
    
    if (pl_len > NGHAM_PL_MAX)
    {
        pl_len = NGHAM_PL_MAX;
    }
    
    uint8_t i;
    for(i=0; i<pl_len; i++)
    {
        p->pl[i] = pl[i];
    }
}

//! \} End of ngham_packets implementation group
