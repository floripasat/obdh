/*
 * misc.h
 *
 *  Created on: 27 de abr de 2016
 *      Author: mario
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
