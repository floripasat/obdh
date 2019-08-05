/*
 * debug_config.h
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
 * \brief Debug configuration parameters.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.14
 * 
 * \date 22/02/2019
 * 
 * \defgroup debug_config Configuration
 * \ingroup debug
 * \{
 */

#ifndef DEBUG_CONFIG_H_
#define DEBUG_CONFIG_H_

#include "../../config.h"

#include "debug.h"

// UART
#define DEBUG_UART_BAUDRATE_BPS         115200

// Mutex config.
#define DEBUG_MUTEX_WAIT_TIME_MS        100

// Debug messages colors
#define DEBUG_SYSTEM_TIME_COLOR         DEBUG_COLOR_GREEN
#define DEBUG_MODULE_NAME_COLOR         DEBUG_COLOR_MAGENTA
#define DEBUG_WARNING_COLOR             DEBUG_COLOR_YELLOW
#define DEBUG_ERROR_COLOR               DEBUG_COLOR_RED

#endif // DEBUG_CONFIG_H_

//! \} End of debug_config group
