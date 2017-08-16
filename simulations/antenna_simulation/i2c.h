/*
 * i2c.c
 *
 *  Created on: 10 de jul de 2017
 *      Author: André
 */

#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include <stdint.h>
#include "antenna.h"
#include "misc_def.h"
#include "temp_data.h"

// I2C functions
void i2c_setup(void);

// I2C definitions
#define I2C_CONTROL_0       UCB0CTL0
#define I2C_CONTROL_1       UCB0CTL1
#define I2C_SW_RESET        UCSWRST
#define I2C_SLAVE_MODE      UCMODE_3
#define I2C_SYNC_MODE       UCSYNC
#define I2C_EN_INTERRUPT    UCB0IE
#define I2C_OWN_ADDRESS     UCB0I2COA
#define SLAVE_ADDRESS_A     0x31
#define SLAVE_ADDRESS_B     0x32

#define SDA_REN     P3REN
#define SDA_OUT     P3OUT
#define SDA_SEL     P3SEL
#define SDA_PIN     BIT0

#define SCL_REN     P3REN
#define SCL_OUT     P3OUT
#define SCL_SEL     P3SEL
#define SCL_PIN     BIT1

#endif /* I2C_H_ */
