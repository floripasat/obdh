
#ifndef __MPU_UART_H_
#define __MPU_UART_H_

#include <msp430.h>
#include <stdio.h>
#include "mpu_uart.h"

void setup_uart(void) {
	P9SEL |= BIT2 + BIT3;							//P9.2,3 = USCI_A2 TXD/RXD
	UCA2CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA2CTL1 |= UCSSEL_1;                     // CLK = ACLK
	UCA2BR0 = 0x03;                        // 32kHz/9600=3.41 (see User's Guide)
	UCA2BR1 = 0x00;                           //
	UCA2MCTL = UCBRS_3 + UCBRF_0;             // Modulation UCBRSx=3, UCBRFx=0
	UCA2CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine
}

void uart_tx(char *tx_data) // Define a function which accepts a character pointer to an array
{
	PTxUart = tx_data;
	UCA2IE |= UCTXIE;                         // Enable USCI_A0 TX interrupt
//	__bis_SR_register(LAM_bits);
}

void float_send(float c) {

	volatile long int d;
	volatile unsigned int hundreds, tens, units, tenths, hundredths,
			thousandths, tenthousandths, thousandth, ten_thousandths = 0;
	volatile long int remainder;
	unsigned char string[30];

	c *= 10000;
	d = (long int) c;
	tens = d / 100000;
	remainder = d - tens * 100000;
	units = remainder / 10000;
	remainder = remainder - units * 10000;
	tenths = remainder / 1000;
	remainder = remainder - tenths * 1000;
	hundredths = remainder / 100;
	remainder = remainder - hundredths * 100;
	thousandth = remainder / 10;
	remainder = remainder - thousandth * 10;
	ten_thousandths = remainder;
	sprintf(string, "%d%d.%d%d", tens, units, tenths, hundredths);
	uart_tx(string);
}

void int_send(int data){
	int i,j,n;
	unsigned char string[7];
	n=1;
	sprintf(string, "%d", data);

	///////// >> string /////////

	//discovering the index
	for(i=0; i < sizeof string;i++){
		if (string[i]==NULL){
			j=i;
			break;
		}
	}
	//shifting to right
	for(i=j;i>=0;i--){
		string[sizeof string - n++]=string[i-1];
	}

	//filling up with blank spaces
	for(i=sizeof string - j - 1;i>=0;i--){
		string[i]=' ';
	}
	/////////(end) >> string /////////

	uart_tx(string);
}

#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void) {
	switch (__even_in_range(UCA2IV, 4)) {
	case 0:
		break;                             // Vector 0 - no interrupt
	case 2:
		break;                             // Vector 2 - RXIFG
	case 4:								   // Vector 4 - TXIFG
		while (*PTxUart) // Increment through array, look for null pointer (0) at end of string
		{
			while ((UCA2STAT & UCBUSY));     // Wait if line TX/RX module is busy with data
			UCA2TXBUF = *PTxUart++; // Send out element i of tx_data array on UART bus
		}
		UCA2IFG &= ~UCTXIFG;
		UCA2IE &= ~UCTXIE;
//		__bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
		break;
	default:
		break;
	}
}

#endif /* MPU_UART_H_ */
