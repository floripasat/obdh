/*! \file i2c.c
    \brief This file handles the msp registers of the I2C interfaces
*/
#include <msp430.h>
#include "i2c.h"


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

void i2c_setup(uint8_t interface) {
    switch(interface)
    {
    case 0:
        BIT_SET(I2C0_SEL, I2C0_SDA | I2C0_SCL);
        i2c_set_clock(USCI_B0_BASE);
        i2c_set_slave(USCI_B0_BASE, EPS_I2C_SLAVE_ADRESS);
        port_mapping_ucb0();
        i2c_set_mode(USCI_B2_BASE, TRANSMIT_MODE);
        break;
    case 1:
        BIT_SET(I2C1_SEL, I2C1_SDA | I2C1_SCL);
        i2c_set_clock(USCI_B1_BASE);
        i2c_set_slave(USCI_B1_BASE, IMU0_I2C_SLAVE_ADRESS);
        i2c_set_mode(USCI_B2_BASE, TRANSMIT_MODE);
        break;
    case 2:
        BIT_SET(I2C2_SEL, I2C2_SDA | I2C2_SCL);
        i2c_set_clock(USCI_B2_BASE);
        i2c_set_slave(USCI_B2_BASE, ANTENNA_SYSTEM_I2C_SLAVE_ADDRESS);
        i2c_set_mode(USCI_B2_BASE, TRANSMIT_MODE);
        break;
    }
}

void i2c_set_clock(uint16_t base_address) {
    HWREG8(base_address + OFS_UCBxCTL1) |= UCSWRST;                     /**< Enable SW reset                    */
    HWREG8(base_address + OFS_UCBxCTL0)  = UCMST | UCMODE_3 | UCSYNC;   /**< I2C Master, synchronous mode       */
    HWREG8(base_address + OFS_UCBxCTL1)  = UCSSEL_2 | UCSWRST;          /**< Use SMCLK, keep SW reset           */
    HWREG8(base_address + OFS_UCBxBR0)   = 160;                         /**< fSCL = SMCLK/160 = ~100kHz         */
    HWREG8(base_address + OFS_UCBxBR1)   = 0;
    HWREG8(base_address + OFS_UCBxCTL1) &= ~UCSWRST;                    /**< Clear SW reset, resume operation   */
}

void i2c_set_mode(uint16_t base_address, uint8_t mode) {
    HWREG8(base_address + OFS_UCBxCTL1) &= ~UCTR;
    HWREG8(base_address + OFS_UCBxCTL1) |= mode;
}


void i2c_set_slave(uint16_t base_address, uint8_t slave_address) {
    HWREG16(base_address + OFS_UCBxI2CSA) = slave_address;
}

void i2c_send(uint16_t base_address, uint8_t tx_data, uint8_t start_stop_flag) {
    uint16_t timeout = 0;

    if(!(start_stop_flag & NO_START))
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTT; //começa a transmissao

    while((!(HWREG8(base_address + OFS_UCBxIFG) & UCTXIFG)) && timeout++ < I2C_TIMEOUT); //UCTXIFG is set again as soon as the data is transferred from the buffer into the shift register
    HWREG8(base_address + OFS_UCBxTXBUF) = tx_data; //envia o byte atual
    while((!(HWREG8(base_address + OFS_UCBxIFG) & UCTXIFG)) && timeout++ < I2C_TIMEOUT); //wait for finish the transmissions


    if(!(start_stop_flag & NO_STOP))
    {
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTP; //Send stop condition.
        HWREG8(base_address + OFS_UCBxIFG) &= ~(UCTXIFG);
    }
}

void i2c_send_burst(uint16_t base_address, uint8_t *p_tx_data, uint16_t bytes) {
    uint16_t timeout = 0;

    HWREG8(base_address + OFS_UCBxCTL1) |= UCTR | UCTXSTT; //começa a transmissao
    while(bytes--)
    {
        while(!(HWREG8(base_address + OFS_UCBxIFG) & UCTXIFG)); //UCTXIFG is set again as soon as the data is transferred from the buffer into the shift register
        HWREG8(base_address + OFS_UCBxTXBUF) = *(p_tx_data++); //envia o byte atual e aponta para o proximo byte
    }

    while((!(HWREG8(base_address + OFS_UCBxIFG) & UCTXIFG)) && timeout++ < I2C_TIMEOUT); //wait for finish the transmissions

//    HWREG8(baseAddress + OFS_UCBxIFG) &= ~(UCTXIFG); //UCTXIFG is automatically reset if a character is written to UCBxTXBUF

    HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTP; //Send stop condition.
}

void i2c_clear_flags(uint16_t base_address) {
    HWREG8(base_address + OFS_UCBxIFG) &= ~UCRXIFG & ~UCTXIFG;
}

uint8_t i2c_receive(uint16_t base_address, uint8_t start_stop_flag) {
    uint16_t timeout = 0;
    uint8_t rx_data;

    if(!(start_stop_flag & NO_START))
    {
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTT;        //send start
        while((HWREG8(base_address + OFS_UCBxCTL1) & UCTXSTT) && timeout++ < I2C_TIMEOUT);    //wait Slave Address ACK
    }

    if(!(start_stop_flag & NO_STOP))
        HWREG8(base_address + OFS_UCBxCTL1) |= UCTXSTP;

    while((!(HWREG8(base_address + OFS_UCBxIFG) & UCRXIFG)) && timeout++ < I2C_TIMEOUT);      //wait to receive data and shift data in buffer
    rx_data = HWREG8(base_address + OFS_UCBxRXBUF);       //receive a byte and increment the pointer


    return rx_data;
}

void i2c_receive_burst(uint16_t base_address, uint8_t *p_rx_data, uint16_t bytes) {
    uint16_t timeout = 0;

    while(bytes--)
    {
        while((!(HWREG8(base_address + OFS_UCBxIFG) & UCRXIFG)) && timeout++ < I2C_TIMEOUT);      //wait to receive data and shift data in buffer
        *(p_rx_data++) = HWREG8(base_address + OFS_UCBxRXBUF);       //receive a byte and increment the pointer
//        HWREG8(baseAddress + OFS_UCBxIFG) &= ~(UCRXIFG);            //UCRXIFG is automatically reset when UCxRXBUF is read.
    }
}
