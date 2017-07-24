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

#include "misc.h"

/*
void sysled_setup(void){
	SYSLED_PORT_DIR |= SYSLED_PIN; // set as output
	SYSLED_PORT_OUT &= ~SYSLED_PIN;
}
void sysled_toggle(void){
	SYSLED_PORT_OUT ^= SYSLED_PIN; //toggle port state
}
void sysled_on(void){
	SYSLED_PORT_OUT |= SYSLED_PIN;
}
void sysled_off(void){
	SYSLED_PORT_OUT &= ~SYSLED_PIN;
}
*/



char* int2str(char* stringBuffer, int16_t value){
	sprintf(stringBuffer, "%d", value);
	return stringBuffer;
}

char* uint2str(char* stringBuffer, uint16_t value){
	sprintf(stringBuffer, "%u", value);
	return stringBuffer;
}

char* float2str(char* stringBuffer, float value) {
	sprintf(stringBuffer, "%f", value);
	return stringBuffer;
}

