/*
 * misc.c
 *
 *  Created on: 27 de abr de 2016
 *      Author: mario
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

