/*
 * spi.h
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
 * \file spi.h
 *
 * \brief This file manage the MSP430's registers of the SPI interfaces
 *
 * \author Arthur Semione
 *
 */
#include <msp430.h>
#include <stdint.h>
#include "../hal/obdh_hal.h"

/**
 * \fn spi_setup
 *
 * \brief Configure pins, clock source and other parameters of the SPI interfaces
 * \return None
 */
void spi_setup(uint8_t interface);

/**
 * \fn spi_tx
 *
 * \brief Send a byte over SPI
 * \param base_address is the MSP registers address of a SPI interface
 * \param data is the byte to be sent
 * \return the received byte
 */
uint8_t spi_tx(uint16_t base_address, uint8_t data);

/**
 * \fn spi_tx_multiple
 *
 * \brief Send 1 or more bytes over SPI
 * \param base_address is the MSP registers address of a SPI interface
 * \param data is a pointer to the first byte to be sent
 * \param data_bytes_length is the amount of bytes to be read
 * \return None
 */
void spi_tx_multiple(uint16_t base_address, uint8_t *data,uint8_t data_bytes_length);

/**
 * \fn spi_rx
 *
 * \brief Receive a byte over SPI
 * \param base_address is the MSP registers address of a SPI interface
 * \return the received byte
 */
#define spi_rx(base_address) spi_tx(base_address, 0xFF)

/**
 * \fn spi_rx
 *
 * \brief Receive 1 or more bytes over SPI
 * \param base_address is the MSP registers address of a SPI interface
 * \param data is a pointer to the first byte to be read
 * \param data_bytes_length is the amount of bytes to be read
 * \return None
 */
void spi_rx_multiple(uint16_t base_address, uint8_t *data, uint8_t data_bytes_length);
