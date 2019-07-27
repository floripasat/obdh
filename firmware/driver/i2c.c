/*
 * i2c.c
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
 * \brief This file manage the MSP430's registers of the I2C interfaces
 *
 * \author Elder Tramontin
 *
 * \version 0.2.6
 *
 * \addtogroup i2c
 */

#include <msp430.h>

#include "../interface/debug/debug.h"

#include "i2c.h"

#define I2C_TIMEOUT                 10000   /**< tries until forsake the current transmission  */

void port_mapping_ucb0(void) {
    // Enable Write-access to modify port mapping registers
    PMAPPWD = 0x02D52;

    // Allow reconfiguration during runtime
    PMAPCTL = PMAPRECFG;

    P2MAP1 = PM_UCB0SDA;
    P2MAP2 = PM_UCB0SCL;

    // Disable Write-Access to modify port mapping registers
    PMAPPWD = 0;
}

void i2c_pin_reset(volatile unsigned char* PREN, volatile unsigned char* PDIR, volatile unsigned char* POUT, uint8_t PIN) {
    uint8_t i;
    BIT_CLEAR(*PREN, PIN);           // disable internal pull-up/down resistor
    BIT_CLEAR(*POUT, PIN);           // output in low level
    BIT_SET(*PDIR, PIN);             // set the pin as output pin

    for(i = 0; i < 20; i++) {
        BIT_TOGGLE(*PDIR, PIN);      // toggle the pin between input(high->external pull-up) / output(low)
        __no_operation();
    }

    BIT_CLEAR(*PDIR, PIN);           // set the pin as input pin (high impedance, external pull-up)
}

void i2c_setup(uint8_t interface) {
    switch(interface) {
        case 0:
            debug_print_event_from_module(DEBUG_INFO, "I2C", "Initializing interface 0 (100 kHz)...");
            debug_new_line();

            i2c_pin_reset(&I2C0_REN, &I2C0_DIR, &I2C0_OUT, I2C0_SCL);
            BIT_SET(I2C0_SEL, I2C0_SDA | I2C0_SCL);
            i2c_set_clock(USCI_B0_BASE);
            i2c_set_slave(USCI_B0_BASE, EPS_I2C_SLAVE_ADRESS);
            port_mapping_ucb0();
            i2c_set_mode(USCI_B0_BASE, TRANSMIT_MODE);
            break;
        case 1:
            debug_print_event_from_module(DEBUG_INFO, "I2C", "Initializing interface 1 (100 kHz)...");
            debug_new_line();

            i2c_pin_reset(&I2C1_REN, &I2C1_DIR, &I2C1_OUT, I2C1_SCL);
            BIT_SET(I2C1_SEL, I2C1_SDA | I2C1_SCL);
            i2c_set_clock(USCI_B1_BASE);
            i2c_set_slave(USCI_B1_BASE, IMU0_I2C_SLAVE_ADRESS);
            i2c_set_mode(USCI_B1_BASE, TRANSMIT_MODE);
            break;
        case 2:
            debug_print_event_from_module(DEBUG_INFO, "I2C", "Initializing interface 2 (100 kHz)...");
            debug_new_line();

            i2c_pin_reset(&I2C2_REN, &I2C2_DIR, &I2C2_OUT, I2C2_SCL);
            BIT_SET(I2C2_SEL, I2C2_SDA | I2C2_SCL);
            i2c_set_clock(USCI_B2_BASE);
            i2c_set_slave(USCI_B2_BASE, ANTENNA_SYSTEM_I2C_SLAVE_ADDRESS);
            i2c_set_mode(USCI_B2_BASE, TRANSMIT_MODE);
            break;
        default:
            debug_print_event_from_module(DEBUG_ERROR, "I2C", "Error initializing interface! Unknown interface number!");
            debug_new_line();
    }
}

void i2c_set_clock(uint16_t base_address) {
    HWREG8(base_address + OFS_UCBxCTL1) |= UCSWRST;                     // Enable SW reset
    HWREG8(base_address + OFS_UCBxCTL0)  = UCMST | UCMODE_3 | UCSYNC;   // I2C Master, synchronous mode
    HWREG8(base_address + OFS_UCBxCTL1)  = UCSSEL_2 | UCSWRST;          // Use SMCLK, keep SW reset
    HWREG8(base_address + OFS_UCBxBR0)   = 160;                         // fSCL = SMCLK/160 = ~100kHz
    HWREG8(base_address + OFS_UCBxBR1)   = 0;
    HWREG8(base_address + OFS_UCBxCTL1) &= ~UCSWRST;                    // Clear SW reset, resume operation
}

void i2c_set_mode(uint16_t base_address, uint8_t mode) {
    HWREG8(base_address + OFS_UCBxCTL1) &= ~UCTR;
    HWREG8(base_address + OFS_UCBxCTL1) |= mode;
}


void i2c_set_slave(uint16_t base_address, uint8_t slave_address) {
    HWREG16(base_address + OFS_UCBxI2CSA) = slave_address;
}

uint8_t i2c_send(uint16_t base_address, uint8_t tx_data, uint8_t start_stop_flag) {
    uint16_t timeout = 0;

    if (!(start_stop_flag & NO_START)) {
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTT;         // starts the transmission
    }

    // UCTXIFG is set again as soon as the data is transferred from the buffer into the shift register
    while((!(HWREG8(base_address + OFS_UCBxIFG) & UCTXIFG)) && timeout++ < I2C_TIMEOUT);

    HWREG8(base_address + OFS_UCBxTXBUF) = tx_data;             // send the current byte

    // wait for finish the transmissions
    while((!(HWREG8(base_address + OFS_UCBxIFG) & UCTXIFG)) && timeout++ < I2C_TIMEOUT);


    if (!(start_stop_flag & NO_STOP)) {
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTP;         // send stop condition
        HWREG8(base_address + OFS_UCBxIFG) &= ~(UCTXIFG);
    }

    if (timeout < I2C_TIMEOUT) {
        return I2C_SUCCESS;                                     // Success
    }
    else {
        return I2C_FAIL;                                        // Fail/Timeout
    }
}

uint8_t i2c_send_burst(uint16_t base_address, uint8_t *p_tx_data, uint16_t bytes, uint8_t start_stop_flag) {
    uint16_t timeout = 0;

    if (!(start_stop_flag & NO_START)) {
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTT;         // starts the transmission
    }

    while(bytes--)
    {
        // UCTXIFG is set again as soon as the data is transferred from the buffer into the shift register
        while(!(HWREG8(base_address + OFS_UCBxIFG) & UCTXIFG)  && (timeout++ < I2C_TIMEOUT));
        HWREG8(base_address + OFS_UCBxTXBUF) = *(p_tx_data++); // send the current byte and points to the next one
    }

    // wait for finish the transmissions
    while((!(HWREG8(base_address + OFS_UCBxIFG) & UCTXIFG)) && timeout++ < I2C_TIMEOUT);

    if (!(start_stop_flag & NO_STOP)) {
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTP;         // send stop condition
        HWREG8(base_address + OFS_UCBxIFG) &= ~(UCTXIFG);
    }

    if (timeout < I2C_TIMEOUT) {
        return I2C_SUCCESS;                                     // Success
    }
    else {
        return I2C_FAIL;                                        // Fail/Timeout
    }
}

void i2c_clear_flags(uint16_t base_address) {
    HWREG8(base_address + OFS_UCBxIFG) &= ~UCRXIFG & ~UCTXIFG;
}

uint8_t i2c_receive(uint16_t base_address, uint8_t *rx_data, uint8_t start_stop_flag) {
    uint16_t timeout = 0;

    if (!(start_stop_flag & NO_START))
    {
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTT;         // starts the transmission
        while((HWREG8(base_address + OFS_UCBxCTL1) & UCTXSTT) && timeout++ < I2C_TIMEOUT);  // wait Slave Address ACK
    }

    if (!(start_stop_flag & NO_STOP)) {
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTP;
    }

    // wait to receive data and shift data on buffer
    while((!(HWREG8(base_address + OFS_UCBxIFG) & UCRXIFG)) && timeout++ < I2C_TIMEOUT);
    *rx_data = HWREG8(base_address + OFS_UCBxRXBUF);            // receive a byte and increment the pointer

    if (timeout < I2C_TIMEOUT) {
        return I2C_SUCCESS;                                     // Success
    }
    else {
        return I2C_FAIL;                                        // Fail/Timeout
    }
}

uint8_t i2c_receive_burst(uint16_t base_address, uint8_t *p_rx_data, uint16_t bytes, uint8_t start_stop_flag) {
    uint16_t timeout = 0;

    if (!(start_stop_flag & NO_START))
    {
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTT;         // starts the transmission
        while((HWREG8(base_address + OFS_UCBxCTL1) & UCTXSTT) && timeout++ < I2C_TIMEOUT);    // wait Slave Address ACK
    }

    while(bytes-- > 1) {
        // wait to receive data and shift data on buffer
        while((!(HWREG8(base_address + OFS_UCBxIFG) & UCRXIFG)) && timeout++ < I2C_TIMEOUT);
        *(p_rx_data++) = HWREG8(base_address + OFS_UCBxRXBUF);  // receive a byte and increment the pointer
    }

    if(!(start_stop_flag & NO_STOP))
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTP;

    // wait to receive data and shift data on buffer
    while((!(HWREG8(base_address + OFS_UCBxIFG) & UCRXIFG)) && timeout++ < I2C_TIMEOUT);
    *(p_rx_data++) = HWREG8(base_address + OFS_UCBxRXBUF);      // receive a byte and increment the pointer

    if (timeout < I2C_TIMEOUT) {
        return I2C_SUCCESS;                                     // Success
    }
    else {
        return I2C_FAIL;                                        // Fail/Timeout
    }
}

//! \} End of i2c group
