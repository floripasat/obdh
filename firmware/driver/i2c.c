#include <driver/i2c.h>

//void i2c_setup(unsigned int device){
//
//	switch(device){
//	case EPS:
//
//		Port_Mapping_UCB0();
//
//		P2SEL |= 0x06;                            // Assign P2.1 to UCB0SDA and...
//		P2DIR |= 0x06;                            // P2.2 to UCB0SCL
//
//		UCB0CTL1 |= UCSWRST;                      // Enable SW reset
//		UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;     // I2C Master, synchronous mode
//		UCB0CTL1 = UCSSEL_2 | UCSWRST;            // Use SMCLK, keep SW reset
//		UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
//		UCB0BR1 = 0;
//		UCB0I2CSA = EPS_I2C_ADRESS;                         // Slave Address
//		UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
//		UCB0IE |= UCRXIE + UCNACKIFG;                         // Enable RX interrupt
//
//		break;
//
//	case MPU:
//
//		P8SEL |= BIT5 + BIT6;                            // Assign P2.1 to UCB0SDA and...
//
//		UCB1CTL1 |= UCSWRST;                      // Enable SW reset
//		UCB1CTL0 = UCMST | UCMODE_3 | UCSYNC;     // I2C Master, synchronous mode
//		UCB1CTL1 = UCSSEL_2 | UCSWRST;            // Use SMCLK, keep SW reset
//		UCB1BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
//		UCB1BR1 = 0;
//		UCB1I2CSA = MPU_I2C_ADRESS;               // Slave Address
//		UCB1CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
//		UCB1IE |= UCTXIE | UCRXIE;                         // Enable RX interrupt
//
//		break;
//	}
//}

void vI2cSetup(uint16_t usBaseAddress, uint8_t ucSlaveAddress)
{
    //Initialize Master
    USCI_B_I2C_initMasterParam param = {0};
    param.selectClockSource = USCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = UCS_getSMCLK();
    param.dataRate = USCI_B_I2C_SET_DATA_RATE_400KBPS;
    USCI_B_I2C_initMaster(usBaseAddress, &param);

    //Specify slave address
    USCI_B_I2C_setSlaveAddress(usBaseAddress, ucSlaveAddress);

    //Set Transmit mode
    USCI_B_I2C_setMode(usBaseAddress, USCI_B_I2C_TRANSMIT_MODE);

    USCI_B_I2C_disableInterrupt(usBaseAddress, 0xFF); //disable all USCI_B0 interrupts

    //Enable I2C Module to start operations
    USCI_B_I2C_enable(usBaseAddress);
}

void vI2cSetMode(uint16_t usBaseAddress, uint8_t ucMode)
{
    USCI_B_I2C_setMode(usBaseAddress, ucMode);
}


void vI2cSetSlave(uint16_t usBaseAddress, uint8_t ucSlaveAddress)
{
    USCI_B_I2C_setSlaveAddress(usBaseAddress, ucSlaveAddress);
}

//device, data address, bytes
bool vI2cWrite(uint16_t usBaseAddress, uint8_t *pucData, uint8_t ucLenght)
{
    volatile uint8_t ucCont;
    volatile bool status;

    USCI_B_I2C_masterSendStart(usBaseAddress);
    for(ucCont = 0; ucCont < ucLenght; ucCont++)
    {
        status = USCI_B_I2C_masterSendMultiByteNextWithTimeout(usBaseAddress, pucData[ucCont], I2C_TIMEOUT);
    }
    //if some send returns STATUS_FAIL, status value will be STAUS_FAIL
    status &=  USCI_B_I2C_masterSendMultiByteStopWithTimeout(usBaseAddress, I2C_TIMEOUT);

    return status;
}

void vI2cRead(uint16_t usBaseAddress, uint8_t ucRegAddress, uint8_t *pucData, uint8_t ucLenght)
{
    volatile uint8_t ucCont;

    USCI_B_I2C_masterSendStart(usBaseAddress);
    USCI_B_I2C_masterSendSingleByteWithTimeout(usBaseAddress, ucRegAddress, I2C_TIMEOUT);
    USCI_B_I2C_setMode(usBaseAddress, USCI_B_I2C_RECEIVE_MODE);
    USCI_B_I2C_masterSendStart(usBaseAddress);
    for(ucCont = 0; ucCont < ucLenght; ucCont++)
    {
//        USCI_B_I2C_masterSendStart(usBaseAddress);
        pucData[ucCont] = USCI_B_I2C_masterReceiveMultiByteNext(usBaseAddress);
    }
    USCI_B_I2C_masterReceiveMultiByteFinish(usBaseAddress);
    USCI_B_I2C_setMode(usBaseAddress, USCI_B_I2C_TRANSMIT_MODE);
}

void Port_Mapping_UCB0(void) {
	// Disable Interrupts before altering Port Mapping registers
	__disable_interrupt();
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
	__enable_interrupt();                     // Re-enable all interrupts
//#endif
}


