/*
 * flash.c
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
 * \file flash.c
 *
 * \brief Functions to use the MSP430's flash memory
 *
 * \author Matheus Frata
 *
 */


#include "flash.h"

void flash_write(uint8_t* data, uint16_t bytes){
    uint16_t i;

    if(FCTL3 & LOCKA) {
        FCTL3 = FWKEY | LOCKA;                        // Clear Lock bit and LockA
    }
    else {
        FCTL3 = FWKEY;                                // Clear Lock bit
    }
    FCTL1 = FWKEY | WRT;                      // Set WRT bit for write operation
    for (i = 0; i < bytes; i++){
        *flash_ptr++ = data[i];               // Write value to flash
        while((FCTL3 & BUSY) == TRUE);        // Check if Flash being used
    }
    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY | LOCK | LOCKA;             // Set LOCK bit
}

void flash_write_single(uint8_t data, uint8_t *addr){
    if(FCTL3 & LOCKA) {
        FCTL3 = FWKEY | LOCKA;                        // Clear Lock bit and LockA
    }
    else {
        FCTL3 = FWKEY;                                // Clear Lock bit
    }
    FCTL1 = FWKEY | WRT;                      // Set WRT bit for write operation
    *addr = data;                             // Write value to flash
    while((FCTL3 & BUSY) == TRUE);            // Check if Flash being used
    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY | LOCK | LOCKA;             // Set LOCK bit
}

void flash_write_long(uint32_t data, uint32_t *addr){

    if(FCTL3 & LOCKA) {
        FCTL3 = FWKEY | LOCKA;                        // Clear Lock bit and LockA
    }
    else {
        FCTL3 = FWKEY;                                // Clear Lock bit
    }
    FCTL1 = FWKEY | BLKWRT;                   // Set WRT bit for write operation
    *addr = data;                             // Write value to flash
    while((FCTL3 & BUSY) == TRUE);            // Check if Flash being used
    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY | LOCK | LOCKA;             // Set LOCK bit

}

void flash_erase(uint32_t* region){
    uint32_t *erase_ptr = region;
    if(FCTL3 & LOCKA) {
        FCTL3 = FWKEY | LOCKA;                        // Clear Lock bit and LockA
    }
    else {
        FCTL3 = FWKEY;                                // Clear Lock bit
    }
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
    FCTL1 = FWKEY;                                // Clear WRT bit
    FCTL3 = FWKEY | LOCK | LOCKA;                 // Set LOCK bit

}


uint8_t flash_read_single(uint8_t *addr){
    return *addr;
}

uint32_t flash_read_long(uint32_t *addr){
    return *addr;
}
