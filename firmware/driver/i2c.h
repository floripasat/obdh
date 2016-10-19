#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include "misc.h"
#include "../driverlib/MSP430F5xx_6xx/driverlib.h"
#include "obdh_hal.h"


#define I2C_TIMEOUT 1000

#define TRANSMIT_MODE                                           UCTR
#define RECEIVE_MODE                                            0x00


unsigned char *PRxData;                     // Pointer to RX data
unsigned char *PTxData;                     // Pointer to TX data
unsigned char RXByteCtr;
unsigned char TXByteCtr;

void Port_Mapping_UCB0(void);
void i2c_setup(unsigned int);
void i2c_read_eps(char *, unsigned int);
void vI2cSetup(uint16_t usBaseAddress, uint8_t ucSlaveAddress);
void vI2cSetSlave(uint16_t usBaseAddress, uint8_t ucSlaveAddress);
void vI2cSetMode(uint16_t usBaseAddress, uint8_t ucMode);
bool vI2cWrite(uint16_t usBaseAddress, uint8_t *pucData, uint8_t ucLenght);    //device, data address, bytes
void vI2cRead(uint16_t usBaseAddress, uint8_t ucRegAddress, uint8_t *pucData, uint8_t ucLenght);

#endif /* I2C_H_ */
