/*
 * misc.h
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
 * \file misc.h
 *
 * \brief This file contains some miscellaneous functions
 *
 * \author Mario Baldini
 *
 */

#ifndef MISC_H_
#define MISC_H_

#include <stdio.h>
#include <stdint.h>

#define TRUE  1
#define FALSE 0

/*
void sysled_setup(void);
void sysled_toggle(void);
void sysled_on(void);
void sysled_off(void);
*/


char*   int2str(char* strbuff,  int16_t value);
char*  uint2str(char* strbuff, uint16_t value);
char* float2str(char* strbuff,    float value);




#endif /* MISC_H_ */
