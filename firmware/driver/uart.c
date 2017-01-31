#include <driver/uart.h>


void uart0_setup(unsigned long baudrate){
	//User Guide Pg. 953

	BIT_SET(UART0_SEL, UART0_TX | UART0_RX);

	UCA2CTL1 |= UCSWRST;							//**Put state machine in reset**
	uart_set_baudrate(baudrate);
	//UCA2IE |= UCRXIE;								//enabling RX interruption
	UCA2CTL1 &= ~UCSWRST;							//**Initialize USCI state machine
}


void uart_set_baudrate(unsigned long baudrate){
	switch(baudrate){
	case 9600:
		UCA2CTL1 |= UCSSEL__ACLK;							//Use ACLK (Auxilliary Clock)
		UCA2BR0 |= 0x03;								//32kHz/9600 = 3.41 (see User's Guide)
		UCA2BR1 |= 0x00;
		UCA2MCTL = UCBRS_3 + UCBRF_0;					//Modulation UCBRSx = 3, UCBRF = 0
		break;
	case 115200:
		UCA2CTL1 |= UCSSEL__SMCLK;                      //TODO: verificar frequencia
		UCA2BR0 |= 0x0A;								//@Clock 16000000
		UCA2BR1 |= 0x08;
		UCA2MCTL = UCBRS_7 + UCBRF_0;
		break;
	default: //9600
		UCA2BR0 |= 0x03;								//32kHz/9600 = 3.41 (see User's Guide)
		UCA2BR1 |= 0x00;
		break;
	}
}

void uart_tx(char *tx_data){					//Define a function that accepts a character pointer to an array
	while (*tx_data != 0) { 					// Increment through array, look for null pointer (0)  at end of string
		while ((UCA2STAT & UCBUSY) == TRUE);    // Wait if line TX/RX module is busy with data
		UCA2TXBUF = *tx_data; 					// Send out element i of tx_data array on UART bus
		tx_data++;
	}
}

void uart_tx_char(char tx_char){
	while ((UCA2STAT & UCBUSY) == TRUE);
	UCA2TXBUF = tx_char;
}

void uart_rx(char *rx_data, int length){
    while (length--) {
        while (!(UCA2IFG & UCRXIFG));
        *rx_data =  UCA2RXBUF;
        rx_data++;
    }
}

char uart_rx_char(){
    while (!(UCA2IFG & UCRXIFG));               //Enquanto a flag for 0
    return UCA2RXBUF;
}
