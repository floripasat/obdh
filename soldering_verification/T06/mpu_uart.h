#ifndef MPU_UART_H_
#define MPU_UART_H_

volatile unsigned char *PTxUart;

void setup_uart(void);
void uart_tx(char *tx_data);
void float_send(float c);
void int_send(int data);

#endif /* MPU_UART_H_ */
