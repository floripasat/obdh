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

#include "mem.h"

#define DUMMY 0xFF

void mem_disable(uint8_t mem_number){

	switch(mem_number){
        case 0:
            Mem0_CE_OUT |= Mem0_CE_PIN;
            break;
        case 1:
            Mem1_CE_OUT |= Mem1_CE_PIN;
            break;
        case 2:
            Mem2_CE_OUT |= Mem2_CE_PIN;
            break;
	}
}

void mem_enable(uint8_t mem_number){

	switch(mem_number){
        case 0:
            Mem0_CE_OUT &= ~Mem0_CE_PIN;
            break;
        case 1:
            Mem1_CE_OUT &= ~Mem1_CE_PIN;
            break;
        case 2:
            Mem2_CE_OUT &= ~Mem2_CE_PIN;
            break;
	}
}

void mem_pulse(uint8_t mem_number) {

	switch(mem_number) {
        case 0:
            Mem0_CE_OUT |= Mem0_CE_PIN;
            Mem0_CE_OUT &= ~Mem0_CE_PIN;
            break;
        case 1:
            Mem1_CE_OUT |= Mem1_CE_PIN;
            Mem1_CE_OUT &= ~Mem1_CE_PIN;
            break;
        case 2:
            Mem2_CE_OUT |= Mem2_CE_PIN;
            Mem2_CE_OUT &= ~Mem2_CE_PIN;
            break;
	}
}

uint8_t mem_status(uint8_t mem_number) {

	mem_enable(mem_number);

	spi_tx(NV_MEM_BASE_ADDRESS, MEM_RDSR);
	spi_tx(NV_MEM_BASE_ADDRESS, DUMMY);

	mem_disable(mem_number);

	return UCA1RXBUF;

}

void mem_id(uint8_t *data_read, uint8_t mem_number) {

	mem_pulse(mem_number);

	spi_tx(NV_MEM_BASE_ADDRESS, MEM_RDID);

	uint8_t i;
	for(i=0;i<3;i++) {
		*data_read = spi_rx(NV_MEM_BASE_ADDRESS);

		data_read++;

	}
	mem_pulse(mem_number);
}

void pin_setup(){

	BIT_SET(Mem_WP_DIR, Mem_WP_PIN | Mem0_CE_PIN | Mem1_CE_PIN | Mem2_CE_PIN );
	BIT_SET(Mem_WP_OUT, Mem_WP_PIN | Mem0_CE_PIN | Mem1_CE_PIN | Mem2_CE_PIN );
}

void mem_WREN(uint8_t mem_number) {

	mem_enable(mem_number);
	spi_tx(NV_MEM_BASE_ADDRESS,MEM_WREN);
	mem_disable(mem_number);
}

void mem_WRDI(uint8_t mem_number) {

	mem_enable(mem_number);
	spi_tx(NV_MEM_BASE_ADDRESS,MEM_WRDI);
	mem_disable(mem_number);
}

void mem_pp(uint8_t data, uint32_t mem_addr, uint8_t mem_number ) {

	mem_WREN(mem_number);

	mem_enable(mem_number);

	spi_tx(NV_MEM_BASE_ADDRESS,MEM_PP);


	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 16) & 0xFF);


	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 8) & 0xFF);


	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr & 0xFF));


	spi_tx(NV_MEM_BASE_ADDRESS,data);

	mem_disable(mem_number);

	while(((mem_status(mem_number)) & BIT0));

}

void mem_read(uint8_t* data_read, uint32_t mem_addr, uint8_t mem_number) {

	mem_enable(mem_number);

	spi_tx(NV_MEM_BASE_ADDRESS,MEM_READ);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 16) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 8) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,mem_addr & 0xFF);

	*data_read = spi_rx(NV_MEM_BASE_ADDRESS);

	mem_disable(mem_number);

}

void mem_read_multiple(uint8_t *data_read, uint32_t mem_addr, uint8_t data_byte_length, uint8_t mem_number) {

	mem_pulse(mem_number);

	spi_tx(NV_MEM_BASE_ADDRESS,MEM_READ);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 16) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 8) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,mem_addr & 0xFF);

	uint8_t i;

	for(i=0; i < data_byte_length; i++) {
		*data_read = spi_rx(NV_MEM_BASE_ADDRESS) ;
		if(*data_read != 0xFF)
			while(1);
		data_read++;
	}

	mem_pulse(mem_number);
}

void mem_pp_multiple(uint8_t *data,uint32_t mem_addr,uint8_t data_byte_length,uint8_t mem_number) {

//	data_byte_length menor que 256 bytes

	mem_WREN(mem_number);

	spi_tx(NV_MEM_BASE_ADDRESS,MEM_PP);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 16) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 8) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,mem_addr & 0xFF);

	uint8_t i;

	for(i=0;i < data_byte_length;i++) {

		spi_tx(NV_MEM_BASE_ADDRESS,*data);
		data++;
		i++;

	}

	mem_pulse(mem_number);

	while(((mem_status(mem_number)) & BIT0));
}

void mem_SER(uint32_t mem_addr, uint8_t mem_number) {


    mem_WREN(mem_number);

    mem_enable(mem_number);

	spi_tx(NV_MEM_BASE_ADDRESS,MEM_SER);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 16) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 8) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr & 0xFF));

	mem_disable(mem_number);

	while(((mem_status(mem_number)) & BIT0));

}

void mem_BER32K(uint32_t mem_addr, uint8_t mem_number) {


    mem_WREN(mem_number);

    mem_enable(mem_number);

	spi_tx(NV_MEM_BASE_ADDRESS,MEM_BER32);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 16) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 8) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,mem_addr & 0xFF);

	mem_disable(mem_number);

	while(((mem_status(mem_number)) & BIT0));

}

void mem_BER64K(uint32_t mem_addr, uint8_t mem_number) {


    mem_WREN(mem_number);

    mem_enable(mem_number);

	spi_tx(NV_MEM_BASE_ADDRESS,MEM_BER64);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 16) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,(mem_addr >> 8) & 0xFF);

	spi_tx(NV_MEM_BASE_ADDRESS,mem_addr & 0xFF);

	mem_disable(mem_number);

	while(((mem_status(mem_number)) & MEM_WIP));

}

void mem_CER(uint8_t mem_number) {

//	Note: A Chip Erase (CER) instruction will be ignored unless all the Block Protection Bits are “0”s.

    mem_WREN(mem_number);

    mem_enable(mem_number);

	spi_tx(NV_MEM_BASE_ADDRESS,MEM_CER);

	mem_disable(mem_number);

	while(((mem_status(mem_number)) & MEM_WIP));

}







