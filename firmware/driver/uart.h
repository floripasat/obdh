/*
 * uart.h
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
 * \file uart.h
 *
 * \brief Functions that deal with the UART interface
 *
 * \author Matheus Frata
 *
 */

#ifndef UART_H_
#define UART_H_

#include <msp430.h>
#include "util/misc.h"
#include "../hal/obdh_hal.h"
#include <stdio.h>

/**
 * \fn uart0_setup
 *
 * \brief Configure the uart UCA2
 * \param baudrate is the uart baudrate
 * \return None
 */
void uart0_setup(unsigned long baudrate);

/**
 * \fn uart_tx
 *
 * \brief Send a string where the last byte is '\0'
 * \param tx_data is a pointer to the first byte
 * \return None
 */
void uart_tx     (char *tx_data);

/**
 * \fn uart_tx_char
 *
 * \brief Send a char
 * \param tx_data is the character to be sent
 * \return None
 */
void uart_tx_char(char  tx_char);

/**
 * \fn uart_tx_bytes
 *
 * \brief Send 1 or more bytes over uart
 * \param tx_data is a pointer to the first byte
 * \param bytes is the amount of bytes to be sent
 * \return None
 */
void uart_tx_bytes(char *tx_data, int16_t bytes);

/**
 * \fn uart_rx
 *
 * \brief Receive 1 or more bytes over uart
 * \param rx_data is a pointer to the first byte
 * \param length is the amount of bytes to be received
 * \return None
 */
void uart_rx(char *rx_data, int length);

/**
 * \fn uart_rx_char
 *
 * \brief Receive a character
 * \return the received character
 */
char uart_rx_char();


#ifdef _DEBUG
    #define debug(tx_data)  uart_tx(tx_data)
#else
    #define debug(tx_data)
#endif

#endif
