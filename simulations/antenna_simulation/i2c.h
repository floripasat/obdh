#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include <stdint.h>

#define SLAVE_ADDRESS_A		0x31
#define SLAVE_ADDRESS_B		0x32

void SetupI2C(void);

#endif
