
#ifndef UART_H_
#define UART_H_

#include <msp430.h>
#include "util/misc.h"
#include <stdio.h>


void uart0_setup(unsigned long);
void uart_tx     (char *tx_data);
void uart_tx_char(char  tx_char);
void uart_rx(char *rx_data, int length);
char uart_rx_char();


void uart_set_baudrate(unsigned long);



#endif
