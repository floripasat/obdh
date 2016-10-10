#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include "misc.h"
#include "../driver/hal.h"

#define MPU 	1
#define EPS 	0

#define MPU_I2C_ADRESS		0x68
#define EPS_I2C_ADRESS		0x13

unsigned char *PRxData;                     // Pointer to RX data
unsigned char *PTxData;                     // Pointer to TX data
unsigned char RXByteCtr;
unsigned char TXByteCtr;

void Port_Mapping_UCB0(void);
void i2c_setup(unsigned int);
void i2c_read_eps(char *, unsigned int);



#endif /* I2C_H_ */
