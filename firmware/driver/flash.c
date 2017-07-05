/*
 * flash.c
 *
 *  Created on: 16 de mai de 2016
 *      Author: mfrata
 */


#include "flash.h"

void flash_write(uint8_t* data, uint16_t bytes){
    uint16_t i;
//    __disable_interrupt();
    FCTL3 = FWKEY;                            // Clear Lock bit
    FCTL1 = FWKEY|WRT;                        // Set WRT bit for write operation
    for (i = 0; i < bytes; i++){
        *flash_ptr++ = data[i];                	// Write value to flash
        while((FCTL3 & BUSY) == TRUE);             // Check if Flash being used
    }
    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY | LOCK | LOCKA;                       // Set LOCK bit
//    __enable_interrupt();
}

void flash_write_single(uint8_t data, uint8_t *addr){
    //  __disable_interrupt();
    FCTL3 = FWKEY;                            // Clear Lock bit
    FCTL1 = FWKEY|WRT;                        // Set WRT bit for write operation
    *addr = data;                				// Write value to flash
    while((FCTL3 & BUSY) == TRUE);            // Check if Flash being used
    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY | LOCK | LOCKA;                       // Set LOCK bit
    //  __enable_interrupt();
}

void flash_write_long(uint32_t data, uint32_t *addr){
//    __disable_interrupt();
    FCTL3 = FWKEY;                            // Clear Lock bit
    FCTL1 = FWKEY|BLKWRT;                     // Set WRT bit for write operation
    *addr = data;                				// Write value to flash
    while((FCTL3 & BUSY) == TRUE);            // Check if Flash being used
    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY | LOCK | LOCKA;                       // Set LOCK bit
//    __enable_interrupt();
}

void flash_erase(uint32_t* region){
	uint32_t *erase_ptr = region;
//	__disable_interrupt();
	FCTL3 = FWKEY;                            // Clear Lock bit
	switch ((uint32_t)region){
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
	FCTL3 = FWKEY | LOCK | LOCKA;                       // Set LOCK bit
//	__enable_interrupt();
}


uint8_t flash_read_single(uint8_t *addr){
    return *addr;
}

uint32_t flash_read_long(uint32_t *addr){
    return *addr;
}
