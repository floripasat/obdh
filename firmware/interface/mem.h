/*
 * mem.c
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
 * \file mem.c
 *
 * \brief This file deals with the flash memory module 25LP128
 *
 * \author Arthur Semione
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "../driver/spi.h"


/* Instructions */

/*      Command        Value      N Description             Addr Dummy  Data  */
#define MEM_WREN       0x06     /* 1 Write Enable              0   0     0     */
#define MEM_WRDI       0x04     /* 1 Write Disable             0   0     0     */
#define MEM_RDID       0x9f     /* 1 Read Identification       0   0     1-3   */
#define MEM_RDSR       0x05     /* 1 Read Status Register      0   0     >=1   */
#define MEM_WRSR       0x01     /* 1 Write Status Register     0   0     1     */
#define MEM_READ       0x03     /* 1 Read Data Bytes           3   0     >=1   */
#define MEM_PP         0x02     /* 1 Page Program              3   0     1-256 */
#define MEM_SER        0x20     /* 1 Sector Erase              3   0     0     */
#define MEM_BER32      0x52     /* 2 32K Block Erase           3   0     0     */
#define MEM_BER64      0xD8     /* 2 64K Block Erase           3   0     0     */
#define MEM_CER        0xC7     /* 1 Chip Erase                0   0     0     */

#define MEM_WIP              (1 << 0)                /* Bit 0: Write in progress bit */
#define MEM_WEL              (1 << 1)                /* Bit 1: Write enable latch bit */

// Functions

uint8_t mem_status(uint8_t mem_number);
void mem_id(uint8_t *data_read, uint8_t mem_number);

void pin_setup();

void mem_pp(uint8_t data, uint32_t mem_addr,uint8_t mem_number);
void mem_pp_multiple(uint8_t *data,uint32_t mem_addr,uint8_t data_byte_length,uint8_t mem_number);

void mem_read_multiple(uint8_t *data_read, uint32_t mem_addr, uint8_t data_byte_length, uint8_t mem_number);
void mem_read(uint8_t *data_read, uint32_t mem_addr,uint8_t mem_number);

void mem_pulse(uint8_t mem_number);
void mem_enable(uint8_t mem_number);
void mem_disable(uint8_t mem_number);

void mem_WRDI(uint8_t mem_number);
void mem_WREN(uint8_t mem_number);
//void mem_WRSR(uint8_t mem_number, uint8_t data );
void mem_SER(uint32_t mem_addr, uint8_t mem_number);
void mem_BER32K(uint32_t mem_addr, uint8_t mem_number);
void mem_BER64K(uint32_t mem_addr, uint8_t mem_number);
void mem_CER(uint8_t mem_number);


