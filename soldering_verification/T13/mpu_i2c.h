/*
 * mpu_i2c.h
 *
 *  Created on: 18/05/2016
 *      Author: ruanmolgero
 */

#ifndef MPU_I2C_H_
#define MPU_I2C_H_

unsigned char *PTxData;                     // Pointer to TX data
char *PRxData;                     // Pointer to RX data
unsigned char RXBuffer;
char TXByteCtr;
char RXByteCtr;

void write_i2c(unsigned char reg_adrr, unsigned char data);
void setup_i2c(unsigned char dev_adrr);
void read_i2c(unsigned char reg_adrr, char buffer[],unsigned int bytes);
void clear(void);

#endif /* MPU_I2C_H_ */
