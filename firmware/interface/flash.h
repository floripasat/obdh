/*
 * flash.h
 *
 *  Created on: 16 de mai de 2016
 *      Author: mfrata
 */

#include <msp430.h>
#include "../driver/obdh_hal.h"
#include "debug.h"

#ifndef UTIL_FLASH_H_
#define UTIL_FLASH_H_

#define FULL 1
#define NOT_FULL 0

char *flash_ptr;                         	// Initialize Flash pointer
long *current_flash_ptr;					//place holder to save flash pointer to boot

void flash_write(char*, int);
void flash_setup(void);
void flash_erase(long*);
void flash_write_single(char ,long *);
void flash_write_long(long* ,long *);
void flash_save_ptr(void);
void flash_reset_ptr(long reset_address);
void flash_ptr_set_new_addr(long next_bank_adress);
unsigned int get_current_bank(void);
unsigned int check_bank_full(int bank);

void write2Flash(char*, int);


#endif /* UTIL_FLASH_H_ */
