/*
 * solar_panel.c
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
 * \file solar_panel.c
 *
 * \brief Interface to deals with Solar Panel board modules
 *
 * \author Andre Mattos
 *
 */

#include "solar_panel.h"

void solar_panel_setup(void) {

    BIT_SET(TEMP_X_CSn_OUT, TEMP_X_CSn_PIN);
    BIT_SET(TEMP_Y_CSn_OUT, TEMP_Y_CSn_PIN);
    BIT_SET(TEMP_Z_CSn_OUT, TEMP_Z_CSn_PIN);

    BIT_SET(TEMP_X_CSn_DIR, TEMP_X_CSn_PIN);
    BIT_SET(TEMP_Y_CSn_DIR, TEMP_Y_CSn_PIN);
    BIT_SET(TEMP_Z_CSn_DIR, TEMP_Z_CSn_PIN);

    spi_rx(SOLAR_PANEL_BASE_ADDRESS); //dummy read to flush spi bus
    solar_panel_read_temperature(0);
    solar_panel_read_temperature(1);
    solar_panel_read_temperature(2);
}

int16_t solar_panel_read_temperature(uint8_t panel_selection) {

    int16_t temperature_raw;

    BIT_CLEAR(SPI1_MISO_OUT, SPI1_MISO_PIN);

    spi_rx(SOLAR_PANEL_BASE_ADDRESS); //dummy read to flush spi bus

    switch(panel_selection){
        case SOLAR_PANEL_X:
            BIT_CLEAR(TEMP_X_CSn_OUT, TEMP_X_CSn_PIN);
            break;
        case SOLAR_PANEL_Y:
            BIT_CLEAR(TEMP_Y_CSn_OUT, TEMP_Y_CSn_PIN);
            break;
        case SOLAR_PANEL_Z:
            BIT_CLEAR(TEMP_Z_CSn_OUT, TEMP_Z_CSn_PIN);
            break;
    }

    temperature_raw = spi_rx(SOLAR_PANEL_BASE_ADDRESS) << 8;

    temperature_raw |= spi_rx(SOLAR_PANEL_BASE_ADDRESS);

    temperature_raw = (temperature_raw & 0xFFFF) / 32;

    switch(panel_selection){
        case SOLAR_PANEL_X:
            BIT_SET(TEMP_X_CSn_OUT, TEMP_X_CSn_PIN);
            break;
        case SOLAR_PANEL_Y:
            BIT_SET(TEMP_Y_CSn_OUT, TEMP_Y_CSn_PIN);
            break;
        case SOLAR_PANEL_Z:
            BIT_SET(TEMP_Z_CSn_OUT, TEMP_Z_CSn_PIN);
            break;
    }

    BIT_SET(SPI1_MISO_OUT, SPI1_MISO_PIN);

    return temperature_raw;
}
