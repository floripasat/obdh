#ifndef __MPU_I2C_H_
#define __MPU_I2C_H_

#include <msp430.h>
#include "mpu_i2c.h"

void write_i2c(unsigned char reg_adrr, unsigned char data) {
	unsigned char TxData[] = { reg_adrr, data };
	PTxData = (unsigned char *) TxData;
	TXByteCtr = sizeof TxData;
	UCB1CTL1 |= UCTR + UCTXSTT;
	while (UCB1CTL1 & UCTXSTP);
}
void setup_i2c(unsigned char dev_adrr) {
	P8SEL |= BIT5 + BIT6;							//Assign I2C pins to USCI_B1
	UCB1CTL1 |= UCSWRST;                      // Enable SW reset
	UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB1CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB1BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
	UCB1BR1 = 0;
	UCB1I2CSA = dev_adrr;                         // Slave Address is 048h
	UCB1CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
	UCB1IE |= UCTXIE | UCRXIE;  //todo int
}
void read_i2c(unsigned char reg_adrr, char buffer[],unsigned int bytes) {
	PTxData = &reg_adrr;
	TXByteCtr = 1;
	UCB1CTL1 |= UCTR + UCTXSTT;
	while (UCB1CTL1 & UCTXSTP);
	RXByteCtr = bytes;
	PRxData = buffer;
	UCB1CTL1 &= ~UCTR;
	UCB1CTL1 |= UCTXSTT;
	while (UCB1CTL1 & UCTXSTP);
}

void clear(void) {
	PTxData = 0;
	PRxData = 0;
	RXBuffer = 0;
	TXByteCtr = 0;
	RXByteCtr = 0;
	UCB1TXBUF = 0;
}



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
			if (RXByteCtr == 1)                   // Only one byte left?
				UCB1CTL1 |= UCTXSTP;              // Generate I2C stop condition
		} else {
		    UCB1CTL1 |= UCTXSTP;
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

#endif /* MPU_I2C_H_ */
