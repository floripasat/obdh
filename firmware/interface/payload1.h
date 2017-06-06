/*
 * payload1.h
 *
 *  Created on: 01 de jun de 2017
 *      Author: elder
 */

#ifndef PAYLOAD1_INTERFACE_H_
#define PAYLOAD1_INTERFACE_H_

#include "../include/floripasat_def.h"
#include "../driver/i2c.h"


#define PAYLOAD1_COMM_ERROR     0x00
#define PAYLOAD1_ALIVE          0x01

#define PAYLOAD1_DATA_LENGTH    100 //bytes


void payload1_setup(void);
uint8_t payload1_read(uint8_t* data, uint32_t address, uint8_t bytes);
uint8_t payload1_write(uint8_t* data, uint32_t address, uint8_t bytes);
#endif /* PAYLOAD1_INTERFACE_H_ */
