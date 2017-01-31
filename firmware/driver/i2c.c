#include <msp430.h>
#include "i2c.h"


void i2c0_setup(void){
    BIT_SET(I2C0_SEL, I2C0_SDA | I2C0_SCL);

    vI2cSetup(USCI_B0_BASE, EPS_SLAVE_ADDRESS);
    Port_Mapping_UCB0();
}

void i2c1_setup(void){
    BIT_SET(I2C1_SEL, I2C1_SDA | I2C1_SCL);
    vI2cSetup(USCI_B1_BASE, IMU1_SLAVE_ADDRESS);
}

void i2c2_setup(void){
    BIT_SET(I2C2_SEL, I2C2_SDA | I2C2_SCL);
    vI2cSetup(USCI_B2_BASE, ANTENNA_DEPLOY_SLAVE_ADDRESS);
}

void vI2cSetup(uint16_t usBaseAddress, uint8_t ucSlaveAddress) {
    HWREG8(usBaseAddress + OFS_UCBxCTL1) |= UCSWRST;    // Enable SW reset
    HWREG8(usBaseAddress + OFS_UCBxCTL0)  = UCMST | UCMODE_3 | UCSYNC;     // I2C Master, synchronous mode
    HWREG8(usBaseAddress + OFS_UCBxCTL1)  = UCSSEL_2 | UCSWRST;            // Use SMCLK, keep SW reset
    HWREG8(usBaseAddress + OFS_UCBxBR0)   = 40;                            // fSCL = SMCLK/40 = ~100kHz
    HWREG8(usBaseAddress + OFS_UCBxBR1)   = 1; //******* ALTERAR PARA ATENDER TODOS OS MODULOS COMUNICADOS POR I2C**** //
    HWREG16(usBaseAddress + OFS_UCBxI2CSA) = ucSlaveAddress;
    HWREG8(usBaseAddress + OFS_UCBxCTL1) &= ~UCSWRST;                      // Clear SW reset, resume operation
}

void vI2cSetMode(uint16_t usBaseAddress, uint8_t ucMode) {
    HWREG8(usBaseAddress + OFS_UCBxCTL1) &= ~UCTR;
    HWREG8(usBaseAddress + OFS_UCBxCTL1) |= ucMode;
}


void vI2cSetSlave(uint16_t usBaseAddress, uint8_t ucSlaveAddress) {
    HWREG16(usBaseAddress + OFS_UCBxI2CSA) = ucSlaveAddress;
}

void vI2cSend(uint16_t usBaseAddress, uint8_t ucPxData, uint8_t ucWithStartStop) {
    uint16_t timeout = 0;

    if(!(ucWithStartStop & NO_START))
        HWREG8(usBaseAddress + OFS_UCBxCTL1) |= UCTXSTT; //começa a transmissao

    while((!(HWREG8(usBaseAddress + OFS_UCBxIFG) & UCTXIFG)) && timeout++ < 10000); //UCTXIFG is set again as soon as the data is transferred from the buffer into the shift register
    HWREG8(usBaseAddress + OFS_UCBxTXBUF) = ucPxData; //envia o byte atual e aponta para o proximo byte
    while((!(HWREG8(usBaseAddress + OFS_UCBxIFG) & UCTXIFG)) && timeout++ < 10000); //wait for finish the transmissions


    if(!(ucWithStartStop & NO_STOP))
    {
        HWREG8(usBaseAddress + OFS_UCBxCTL1) |= UCTXSTP; //Send stop condition.
        HWREG8(usBaseAddress + OFS_UCBxIFG) &= ~(UCTXIFG);
    }
}

void vI2cSendBurst(uint16_t usBaseAddress, uint8_t *pucPxData, uint16_t usBytes) {
    uint16_t timeout = 0;

    HWREG8(usBaseAddress + OFS_UCBxCTL1) |= UCTR | UCTXSTT; //começa a transmissao
    while(usBytes--)
    {
        while(!(HWREG8(usBaseAddress + OFS_UCBxIFG) & UCTXIFG)); //UCTXIFG is set again as soon as the data is transferred from the buffer into the shift register
        HWREG8(usBaseAddress + OFS_UCBxTXBUF) = *(pucPxData++); //envia o byte atual e aponta para o proximo byte
    }

    while((!(HWREG8(usBaseAddress + OFS_UCBxIFG) & UCTXIFG)) && timeout++ < 10000); //wait for finish the transmissions

//    HWREG8(baseAddress + OFS_UCBxIFG) &= ~(UCTXIFG); //UCTXIFG is automatically reset if a character is written to UCBxTXBUF

    HWREG8(usBaseAddress + OFS_UCBxCTL1) |= UCTXSTP; //Send stop condition.
}

void vI2cClearFlags(uint16_t usBaseAddress) {
    HWREG8(usBaseAddress + OFS_UCBxIFG) &= ~UCRXIFG & ~UCTXIFG;
}

uint8_t vI2cReceive(uint16_t usBaseAddress, uint8_t ucWithStartStop) {
    uint8_t ucPxData;

    if(!(ucWithStartStop & NO_START))
    {
        HWREG8(usBaseAddress + OFS_UCBxCTL1) |= UCTXSTT;        //send start
        while(HWREG8(usBaseAddress + OFS_UCBxCTL1) & UCTXSTT);    //wait Slave Address ACK
    }

    if(!(ucWithStartStop & NO_STOP))
        HWREG8(usBaseAddress + OFS_UCBxCTL1) |= UCTXSTP;

    while(!(HWREG8(usBaseAddress + OFS_UCBxIFG) & UCRXIFG));      //wait to receive data and shift data in buffer
    ucPxData = HWREG8(usBaseAddress + OFS_UCBxRXBUF);       //receive a byte and increment the pointer


    return ucPxData;
}

void vI2cReceiveBurst(uint16_t usBaseAddress, uint8_t *pucPxData, uint16_t usBytes) {
    while(usBytes--)
    {
        while(!(HWREG8(usBaseAddress + OFS_UCBxIFG) & UCRXIFG));      //wait to receive data and shift data in buffer
        *(pucPxData++) = HWREG8(usBaseAddress + OFS_UCBxRXBUF);       //receive a byte and increment the pointer
//        HWREG8(baseAddress + OFS_UCBxIFG) &= ~(UCRXIFG);            //UCRXIFG is automatically reset when UCxRXBUF is read.
    }
}

void Port_Mapping_UCB0(void) {
	// Disable Interrupts before altering Port Mapping registers
//	__disable_interrupt();
	// Enable Write-access to modify port mapping registers
	PMAPPWD = 0x02D52;

//#ifdef PORT_MAP_RECFG
	// Allow reconfiguration during runtime
	PMAPCTL = PMAPRECFG;
//#endif

	P2MAP1 = PM_UCB0SDA;
	P2MAP2 = PM_UCB0SCL;

	// Disable Write-Access to modify port mapping registers
	PMAPPWD = 0;
//#ifdef PORT_MAP_EINT
//	__enable_interrupt();                     // Re-enable all interrupts
//#endif
}


