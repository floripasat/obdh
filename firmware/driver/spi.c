/*
 * spi.c
 *
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina
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
 * \brief This file manage the MSP430's registers of the SPI interfaces
 *
 * \author Arthur Semione
 *
 * \version 0.2.6
 *
 * \addtogroup spi
 */

#include "spi.h"

#include "../interface/debug/debug.h"

#define DUMMY 0xFF

void port_mapping_uca0(void) {
    PMAPPWD = 0x02D52;      // Enable Write-access to modify port mapping registers

    PMAPCTL = PMAPRECFG;    // Allow reconfiguration during runtime

    P2MAP0 = PM_UCA0CLK;
    P2MAP4 = PM_UCA0SIMO;
    P2MAP5 = PM_UCA0SOMI;

    PMAPPWD = 0;            // Disable Write-Access to modify port mapping registers
}

void spi_clock_setup(uint16_t base_address) {
    HWREG8(base_address + OFS_UCAxCTL1) |= UCSWRST;                              // Put state machine in reset
    HWREG8(base_address + OFS_UCAxCTL0) &= ~(UCCKPH + UCCKPL + UC7BIT + UCMSB +
                                                UCMST + UCMODE_3 + UCSYNC);
    HWREG8(base_address + OFS_UCAxCTL0) |= UCMST | UCSYNC | UCCKPH | UCMSB;      // 3-pin, 8-bit SPI master, clock polarity high, MSB

    HWREG8(base_address + OFS_UCAxCTL1) |= UCSSEL_2;                             // use SMCLK
    //HWREG16(base_address + OFS_UCAxBRW) = 2;                                   // SMCLK/2 = 8MHz
    HWREG16(base_address + OFS_UCAxBRW) = 39;                                    // SMCLK/40 = kHz

    HWREG8(base_address + OFS_UCAxMCTL) = 0;                                     // No modulation
    HWREG8(base_address + OFS_UCAxCTL1) &= ~(UCSWRST);                           // Initialize USCI state machine
}

void spi_setup(uint8_t interface) {
    switch(interface) {
        case 0:
            debug_print_event_from_module(DEBUG_INFO, "SPI", "Initializing interface 0 (8 MHz)...");
            debug_new_line();

            BIT_SET(SPI0_SEL, SPI0_CLK_PIN | SPI0_MOSI_PIN | SPI0_MISO_PIN);
            spi_clock_setup(USCI_A0_BASE);
            port_mapping_uca0();
            break;
        case 1:
            debug_print_event_from_module(DEBUG_INFO, "SPI", "Initializing interface 1 (8 MHz)...");
            debug_new_line();

            BIT_SET(SPI1_SEL, SPI1_CLK_PIN | SPI1_MOSI_PIN | SPI1_MISO_PIN);
            spi_clock_setup(USCI_A1_BASE);
            BIT_SET(SPI1_MISO_REN, SPI1_MISO_PIN);                              // Enable pull-up resistor
            BIT_SET(SPI1_MISO_OUT, SPI1_MISO_PIN);
            break;
        default:
            debug_print_event_from_module(DEBUG_ERROR, "SPI", "Error initializing interface! Unknown interface number!");
            debug_new_line();
    }
}

void wait_for_buffer_empty(uint16_t base_address) {
    while (!(HWREG8(base_address + OFS_UCAxIFG) & UCTXIFG));    // flag is set when data are sent/arrived
}

void wait_for_data_sent(uint16_t base_address) {
    while (!(HWREG8(base_address + OFS_UCAxIFG) & UCRXIFG));    // flag is set when data are sent/arrived
}

uint8_t spi_tx(uint16_t base_address, uint8_t data) {
    wait_for_buffer_empty(base_address);
    HWREG8(base_address + OFS_UCAxTXBUF) = data;
    wait_for_data_sent(base_address);

	return (HWREG8(base_address + OFS_UCAxRXBUF));
}

void spi_tx_multiple(uint16_t base_address, uint8_t *data,uint8_t data_bytes_length) {
	uint8_t i;

	for(i=0; i < data_bytes_length; i++) {
		spi_tx(base_address, *data);
		data++;
	}
}

void spi_rx_multiple(uint16_t base_address, uint8_t *data, uint8_t data_bytes_length) {
	uint8_t i;

	for(i = 0; i < data_bytes_length; i++) {
		*data = spi_rx(base_address);
		data++;
    }
}

//! \} End of spi group
