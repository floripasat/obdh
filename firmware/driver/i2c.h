#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include "misc.h"
#include "../driverlib/MSP430F5xx_6xx/inc/hw_memmap.h"
#include "obdh_hal.h"



//#define I2C_TIMEOUT         50

#define TRANSMIT_MODE                                           UCTR
#define RECEIVE_MODE                                            0x00


unsigned char *PRxData;                     // Pointer to RX data
unsigned char *PTxData;                     // Pointer to TX data
unsigned char RXByteCtr;
unsigned char TXByteCtr;

void Port_Mapping_UCB0(void);

void vI2cSetup(uint16_t usBaseAddress, uint8_t ucSlaveAddress);
void vI2cSetSlave(uint16_t usBaseAddress, uint8_t ucSlaveAddress);
void vI2cSetMode(uint16_t usBaseAddress, uint8_t ucMode);
void vI2cSend(uint16_t usBaseAddress, uint8_t ucPxData, uint8_t ucWithStartStop);
void vI2cSendBurst(uint16_t usBaseAddress, uint8_t *pucPxData, uint16_t usBytes);
void vI2cReceiveBurst(uint16_t usBaseAddress, uint8_t *pucPxData, uint16_t usBytes);
uint8_t vI2cReceive(uint16_t usBaseAddress, uint8_t ucWithStartStop);

#define START_STOP  1<<0
#define NO_START 1<<1
#define NO_STOP  1<<2


#endif /* I2C_H_ */
