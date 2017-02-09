#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include "misc.h"
#include "../hal/obdh_hal.h"



//#define I2C_TIMEOUT         50

#define TRANSMIT_MODE                                           UCTR
#define RECEIVE_MODE                                            0x00

#define EPS_SLAVE_ADDRESS 0x48              //TODO: FIX THE ADDRESS
#define IMU1_SLAVE_ADDRESS 0x68
#define ANTENNA_DEPLOY_SLAVE_ADDRESS 0x12   //TODO: FIX THE ADDRESS


unsigned char *PRxData;                     // Pointer to RX data
unsigned char *PTxData;                     // Pointer to TX data
unsigned char RXByteCtr;
unsigned char TXByteCtr;

void port_mapping_ucb0(void);
void i2c_setup(uint8_t interface);
void vI2cSetup(uint16_t usBaseAddress, uint8_t ucSlaveAddress);
void i2c_set_slave(uint16_t usBaseAddress, uint8_t ucSlaveAddress);
void i2c_set_mode(uint16_t usBaseAddress, uint8_t ucMode);
void i2c_send(uint16_t usBaseAddress, uint8_t ucPxData, uint8_t ucWithStartStop);
void i2c_send_burst(uint16_t usBaseAddress, uint8_t *pucPxData, uint16_t usBytes);
void i2c_receive_burst(uint16_t usBaseAddress, uint8_t *pucPxData, uint16_t usBytes);
uint8_t i2c_receive(uint16_t usBaseAddress, uint8_t ucWithStartStop);

#define     START_STOP   (0x0001)
#define     NO_START     (0x0002)
#define     NO_STOP      (0x0004)


#endif /* I2C_H_ */
