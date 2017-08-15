/*
 * ccsds_scrambler.h
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
 * \file ccsds_scrambler.h
 * 
 * \brief .
 * 
 * \author Jon Petter Skagmo <web@skagmo.com>; Mods. for FloripaSat-TTC by Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 27/04/2017
 * 
 * \defgroup ccsds_scrambler CCSDS-Scrambler
 * \ingroup ngham
 * \{
 */

#ifndef CCSDS_SCRAMBLER_H_
#define CCSDS_SCRAMBLER_H_

#include <stdint.h>

/**
 * \brief CCSDS polynomial.
 * 
 * \{
 */
extern const uint8_t ccsds_poly[255];
//! \}

/**
 * \fn ngham_CCSDS_ScramblerInit
 * 
 * \brief
 * 
 * \return None
 */
void ngham_CCSDS_ScramblerInit();

/**
 * \fn ngham_CCSDS_ScramblerXor
 * 
 * \brief 
 * 
 * Data should be Xored with the sequence, starting from index zero.
 * 
 * \param data
 * 
 * \return None
 */
uint8_t ngham_CCSDS_ScramblerXor(uint8_t data);

#endif // CCSDS_SCRAMBLER_H_

//! \} End of ccsds_scrambler group

