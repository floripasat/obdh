/*
 * debug_uart.c
 * 
 * Copyright (C) 2019, Universidade Federal de Santa Catarina
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
 * along with FloripaSat-OBDH. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Debug UART implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.14
 * 
 * \date 22/02/2019
 * 
 * \defgroup debug_uart UART
 * \ingroup debug
 * \{
 */

#include "../../driver/uart.h"

#include "debug.h"
#include "debug_config.h"

bool debug_uart_init()
{
    uart0_setup(DEBUG_UART_BAUDRATE_BPS);

    return true;
}

void debug_uart_write_byte(uint8_t byte)
{
    uart_tx_char(byte);
}

//! \} End of debug_uart group
