#include <driver/i2c.h>

void i2c_setup(unsigned int device){

	switch(device){
	case EPS:

		Port_Mapping_UCB0();

		P2SEL |= 0x06;                            // Assign P2.1 to UCB0SDA and...
		P2DIR |= 0x06;                            // P2.2 to UCB0SCL

		UCB0CTL1 |= UCSWRST;                      // Enable SW reset
		UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;     // I2C Master, synchronous mode
		UCB0CTL1 = UCSSEL_2 | UCSWRST;            // Use SMCLK, keep SW reset
		UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
		UCB0BR1 = 0;
		UCB0I2CSA = EPS_I2C_ADRESS;                         // Slave Address
		UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
		UCB0IE |= UCRXIE + UCNACKIFG;                         // Enable RX interrupt

		break;

	case MPU:

		P8SEL |= BIT5 + BIT6;                            // Assign P2.1 to UCB0SDA and...

		UCB1CTL1 |= UCSWRST;                      // Enable SW reset
		UCB1CTL0 = UCMST | UCMODE_3 | UCSYNC;     // I2C Master, synchronous mode
		UCB1CTL1 = UCSSEL_2 | UCSWRST;            // Use SMCLK, keep SW reset
		UCB1BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
		UCB1BR1 = 0;
		UCB1I2CSA = MPU_I2C_ADRESS;               // Slave Address
		UCB1CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
		UCB1IE |= UCTXIE | UCRXIE;                         // Enable RX interrupt

		break;
	}
}

void i2c_read_eps(char *Buffer, unsigned int bytes){
	PRxData = Buffer;
	RXByteCtr = bytes;
	UCB0CTL1 &= ~UCTR;
	UCB0CTL1 |= UCTXSTT;
//    __delay_cycles(DELAY_10_MS_IN_CYCLES);
//    __delay_cycles(DELAY_10_MS_IN_CYCLES);
//    __delay_cycles(DELAY_10_MS_IN_CYCLES);
	//vTaskDelay(30 / portTICK_PERIOD_MS);
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

/*
 *  USCB_0 interrupt vector *
 */

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void) {
	switch (__even_in_range(UCB0IV, 12)) {
	case 0:
		break;                           // Vector  0: No interrupts
	case 2:
		break;                           // Vector  2: ALIFG
	case 4:                           // Vector  4: NACKIFG
		break;
	case 6:
		break;                           // Vector  6: STTIFG
	case 8:
		break;                           // Vector  8: STPIFG
	case 10:                                  // Vector 10: RXIFG
		RXByteCtr--;                            // Decrement RX byte counter
		if (RXByteCtr > 0) {
			*PRxData++ = UCB0RXBUF;           // Move RX data to address PRxData
			if (RXByteCtr == 1) {                  // Only one byte left?
				UCB0CTL1 |= UCTXSTP;              // Generate I2C stop condition
			}

		} else {
			*PRxData = UCB0RXBUF;               // Move final RX data to PRxData
		}
		break;
	case 12:
		break;                           // Vector 12: TXIFG
	default:
		break;
	}
}

/*
 *  USCB_1 interrupt vector *
 */

#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void) {
	switch (__even_in_range(UCB1IV, 12)) {
	case 0:
		break;                           // Vector  0: No interrupts
	case 2:
		break;                           // Vector  2: ALIFG
	case 4:
		break;                           // Vector  4: NACKIFG
	case 6:
		break;                           // Vector  6: STTIFG
	case 8:
		break;                           // Vector  8: STPIFG
	case 10:                            		// Vector 10: RXIFG
		RXByteCtr--;                            // Decrement RX byte counter
		if (RXByteCtr > 0) {
			*PRxData++ = UCB1RXBUF;           // Move RX data to address PRxData
			if (RXByteCtr == 1){                   // Only one byte left?
				UCB1CTL1 |= UCTXSTP;              // Generate I2C stop condition
			}
		} else {
			*PRxData = UCB1RXBUF;               // Move final RX data to PRxData
			UCB1IFG &= ~UCRXIFG;                  // Clear USCI_B0 TX int flag
		}
		break;
	case 12:                                  // Vector 12: TXIFG
		if (TXByteCtr--)  {                        // Check TX byte counter
			UCB1TXBUF = *PTxData++;               // Load TX buffer
			__delay_cycles(100);
		} else {
			UCB1CTL1 |= UCTXSTP;                  // I2C stop condition
			UCB1IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
		}
		break;
	default:
		break;
	}
}

