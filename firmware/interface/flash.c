/*
 * flash.c
 *
 *  Created on: 16 de mai de 2016
 *      Author: mfrata
 */


#include "flash.h"

void write2Flash(char* data, int bytes) {
    debug("Writing to flash init");
    unsigned int current_bank = get_current_bank();
    unsigned int current_bank_situation = check_bank_full(current_bank);
    if (current_bank_situation == FULL) {
        if (current_bank == BANK1) {
            flash_erase((long*)BANK2_ADDR);
            flash_ptr = (char*)BANK2_ADDR;
        } else if (current_bank == BANK2) {
            flash_erase((long*)BANK3_ADDR);
            flash_ptr = (char*)BANK3_ADDR;
        } else {
            flash_erase((long*)BANK1_ADDR);
            flash_ptr = (char*)BANK1_ADDR;
        }
    }
    flash_write(data, bytes);
    flash_save_ptr();
    debug("Writing to flash done");
}


void flash_write(char* data, int bytes){
	  unsigned int i;
	//  __disable_interrupt();
	  FCTL3 = FWKEY;                            // Clear Lock bit
	  FCTL1 = FWKEY|WRT;                        // Set WRT bit for write operation
	  for (i = 0; i < bytes; i++){
		*flash_ptr++ = data[i];                	// Write value to flash
		while((FCTL3 & BUSY) == TRUE);             // Check if Flash being used
	  }
	  FCTL1 = FWKEY;                            // Clear WRT bit
	  FCTL3 = FWKEY|LOCK;                       // Set LOCK bit
	//  __enable_interrupt();
}

void flash_write_single(char data, long *addr){
//  __disable_interrupt();
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY|WRT;                        // Set WRT bit for write operation
  *addr = data;                				// Write value to flash
  while((FCTL3 & BUSY) == TRUE);            // Check if Flash being used
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY|LOCK;                       // Set LOCK bit
//  __enable_interrupt();
}

void flash_write_long(long* data, long *addr){
//  __disable_interrupt();
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY|BLKWRT;                     // Set WRT bit for write operation
  *addr = (long)data;                				// Write value to flash
  while((FCTL3 & BUSY) == TRUE);            // Check if Flash being used
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY|LOCK;                       // Set LOCK bit
//  __enable_interrupt();
}


void flash_setup(void) {
	current_flash_ptr = (long*)FLASH_PTR_ADDR;
	flash_ptr = (char*)(*current_flash_ptr);
}

void flash_erase(long* region){
	long *erase_ptr = region;
//	__disable_interrupt();
	FCTL3 = FWKEY;                            // Clear Lock bit
	switch ((long)region){
	case BANK0_ADDR: FCTL1 = FWKEY | MERAS; break;
	case BANK1_ADDR: FCTL1 = FWKEY | MERAS; break;
	case BANK2_ADDR: FCTL1 = FWKEY | MERAS; break;
	case BANK3_ADDR: FCTL1 = FWKEY | MERAS; break;
	case SEGA_ADDR:  FCTL1 = FWKEY | ERASE; break;
	case SEGB_ADDR:  FCTL1 = FWKEY | ERASE; break;
	case SEGC_ADDR:  FCTL1 = FWKEY | ERASE; break;
	case SEGD_ADDR:  FCTL1 = FWKEY | ERASE; break;
	case MASS_ERASE: FCTL1 = FWKEY | MERAS | ERASE; break;
	}
	*erase_ptr = 0;
	while((FCTL3 & BUSY) == TRUE);
	FCTL1 = FWKEY;                            	// Clear WRT bit
	FCTL3 = FWKEY | LOCK;                       // Set LOCK bit
//	__enable_interrupt();
}


void flash_save_ptr(void){
	current_flash_ptr = (long*)FLASH_PTR_ADDR;
	flash_erase((long*)FLASH_PTR_ADDR);
	flash_write_long((long*)flash_ptr, (long*)current_flash_ptr);
}

void flash_ptr_set_new_addr(long next_bank_adress){
	flash_erase((long*)FLASH_PTR_ADDR);
	flash_write_long((long*)next_bank_adress,(long*)FLASH_PTR_ADDR);
}

void flash_reset_ptr(long reset_address){
    flash_erase((long*)FLASH_PTR_ADDR);
    flash_write_long((long*)reset_address,(long*)FLASH_PTR_ADDR);
}

unsigned int check_bank_full(int bank) {
    *current_flash_ptr =(long)flash_ptr;
    volatile unsigned long remaining_bytes = 0;
    unsigned long bank_last_adress = 0;
    unsigned int bank_situation = 0;


    switch (bank) {
    case BANK0:
        bank_last_adress = BANK1_ADDR-1;
        break;
    case BANK1:
        bank_last_adress = BANK2_ADDR-1;
        break;
    case BANK2:
        bank_last_adress = BANK3_ADDR-1;
        break;
    case BANK3:
        bank_last_adress = LAST_WRITE_ADDR;
        break;
    }
    remaining_bytes = (bank_last_adress - *current_flash_ptr);
    //TODO alterar UG_FRAME_LENGTH para tamanho do pacote da missao
    /*if (remaining_bytes < UG_FRAME_LENGTH) {
        bank_situation = FULL;
    } else {
        bank_situation = NOT_FULL;
    }*/

    return bank_situation;
}

unsigned int get_current_bank(void) {
    *current_flash_ptr = (long)flash_ptr;
    unsigned int bank;
    if (*current_flash_ptr >= BANK1_ADDR && *current_flash_ptr < BANK2_ADDR) {
        bank = BANK1;
    } else if (*current_flash_ptr >= BANK2_ADDR && *current_flash_ptr < BANK3_ADDR) {
        bank = BANK2;
    } else if (*current_flash_ptr >= BANK3_ADDR && *current_flash_ptr <= LAST_WRITE_ADDR) {
        bank = BANK3;
    }
    return bank;
}

