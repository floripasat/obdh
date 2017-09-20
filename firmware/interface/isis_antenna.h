/*
 * isis_antenna.h
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
 * \file isis_antenna.h
 *
 * \brief This file deals with the ISIS antenna module
 *
 * \author Elder Tramontin
 *
 */

#ifndef INCLUDE_ISIS_ANTENNA_H_
#define INCLUDE_ISIS_ANTENNA_H_

#include <stdint.h>

//MSB
#define ANTENNA_1_STATUS    BITF
#define ANTENNA_1_TIMEOUT   BITE
#define ANTENNA_1_BURNING   BITD
//NULL BIT
#define ANTENNA_2_STATUS    BITB
#define ANTENNA_2_TIMEOUT   BITA
#define ANTENNA_2_BURNING   BIT9
#define IGNORING_SWITCHES   BIT8
//LSB
#define ANTENNA_3_STATUS    BIT7
#define ANTENNA_3_TIMEOUT   BIT6
#define ANTENNA_3_BURNING   BIT5
#define INDEPENDET_BURN     BIT4
#define ANTENNA_4_STATUS    BIT3
#define ANTENNA_4_TIMEOUT   BIT2
#define ANTENNA_4_BURNING   BIT1
#define ARMED               BIT0

#define ANTENNAS_STATUS_MASK    0x8888

//antenna status
#define ANTENNA_NOT_DEPLOYED    1       /**< Value if antennas are not deployed yet */
#define ANTENNA_DEPLOYED        0       /**< Value if antennas are deployed         */

//antenna stop cause
#define TIMEOUT_CAUSE           1       /**< Value if deployment system stops because timeout                   */
#define OTHER_CAUSE             0       /**< Value if deployment system stops because other reason than timeout */

//antenna burn system
#define BURN_ACTIVE             1       /**< Value if the referring antenna burn system is active */
#define BURN_INACTIVE           0       /**< Value if the referring antenna burn system is off    */

#define VERIFY_STATUS(status, bit)   ( (status & bit) > 0 )

#define ANTENNA_1               1
#define ANTENNA_2               2
#define ANTENNA_3               3
#define ANTENNA_4               4

void arm_antenna(void);
void disarm_antenna(void);
void start_sequential_deploy(uint8_t seconds);
void start_independet_deploy(uint8_t antenna, uint8_t seconds, uint8_t override);
uint16_t read_deployment_status(void);

#endif /* INCLUDE_ISIS_ANTENNA_H_ */
