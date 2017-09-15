/*
 * solar_panel.h
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
 * \file solar_panel.h
 *
 * \brief Interface to deals with Solar Panel board modules
 *
 * \author Andre Mattos
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "../hal/obdh_hal.h"
#include "../driver/spi.h"

#define SOLAR_PANEL_X   0
#define SOLAR_PANEL_Y   1
#define SOLAR_PANEL_Z   2

/**
 * \fn solar_panel_setup
 *
 * \brief Configure the IO pins to deals with the solar panel modules
 * \return None
 */
void solar_panel_setup (void);

/**
 * \fn solar_panel_read_temperature
 *
 * \brief Read the temperature of a solar panel module
 * \param panel_selection is the chosen module to be read. SOLAR_PANEL_X,
 * SOLAR_PANEL_Y and SOLAR_PANEL_Z macros can be used.
 * \return None
 */
int16_t solar_panel_read_temperature (uint8_t panel_selection);
