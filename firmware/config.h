/*
 * config.h
 *
 * Copyright (C) 2019, Universidade Federal de Santa Catarina.
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
 * \brief General configuration definitions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 1.0.6
 * 
 * \date 03/07/2019
 *
 * \defgroup config Configuration
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define OBDH_RESET_MEMORY_ON_BOOT               0
#define OBDH_EXECUTE_DEPLOYMENT_ROUTINE         1

#define OBDH_DEBUG_MESSAGES                     1

#define OBDH_TX_ENABLED                         1

#define OBDH_PAYLOAD_X_ENABLED                  0

#define OBDH_TELECOMMAND_ENTER_HIBERNATION_KEY  "69jCwUyK"
#define OBDH_TELECOMMAND_LEAVE_HIBERNATION_KEY  "MbaY2fNG"
#define OBDH_TELECOMMAND_CHARGE_RESET_KEY       "bVCd25Fh"
#define OBDH_TELECOMMAND_ENABLE_RUSH_KEY        "peU9ZGH3"

#endif // CONFIG_H_

//! \} End of config group
