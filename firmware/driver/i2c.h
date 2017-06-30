#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include "misc.h"
#include "../hal/obdh_hal.h"



//#define I2C_TIMEOUT         50

#define TRANSMIT_MODE                   UCTR        /**< Tx mode bit set    */
#define RECEIVE_MODE                    0x00        /**< Tx mode bit clear  */

#define I2C_TIMEOUT                     0xFFFF//10000

#define I2C_SUCESS                      1
#define I2C_FAIL                        0


/**
 * \fn i2c_setup
 *
 * \brief map the i2c (UCB0) pins of the PORT 2
 * \param interface is the I2C target interface (0, 1 or 2)
 * \return None
 */
void i2c_setup(uint8_t interface);

/**
 * \fn i2c_set_clock
 *
 * \brief set the clock source and frequency
 * \param base_address is the I2C registers base address of the serial interface
 * \return None
 */
void i2c_set_clock(uint16_t base_address);

/**
 * \fn i2c_set_slave
 *
 * \brief set the slave address
 * \param base_address is the I2C registers base address of the serial interface
 * \param slave_address is the 8-bit slave address
 * \return None
 */
void i2c_set_slave(uint16_t base_address, uint8_t slave_address);

/**
 * \fn i2c_set_mode
 *
 * \brief set the clock source and frequency
 * \param base_address is the I2C registers base address of the serial interface
 * \param mode is the master mode: Rx or Tx
 * \return None
 */
void i2c_set_mode(uint16_t base_address, uint8_t mode);

/**
 * \fn i2c_send
 *
 * \brief send 1 byte
 * \param base_address is the I2C registers base address of the serial interface
 * \param tx_data is the data to transmit
 * \param start_stop_flag switch if there will be a start and/or stop signal between the transmission
 * \return sucess or timeout
 */
uint8_t i2c_send(uint16_t base_address, uint8_t tx_data, uint8_t start_stop_flag);

/**
 * \fn i2c_send_burst
 *
 * \brief send 1 or more bytes
 * \param base_address is the I2C registers base address of the serial interface
 * \param p_tx_data is a pointer to a data buffer, with the data to send
 * \param bytes is the buffer size, in bytes
 * \param start_stop_flag switch if there will be a start and/or stop signal between the transmission
 * \return sucess or timeout
 */
uint8_t i2c_send_burst(uint16_t base_address, uint8_t *p_tx_data, uint16_t bytes, uint8_t start_stop_flag);

/**
 * \fn i2c_receive_burst
 *
 * \brief receive 1 or more bytes
 * \param base_address is the I2C registers base address of the serial interface
 * \param p_rx_data is a pointer to a data buffer, to store the received data
 * \param bytes is the buffer size, in bytes
 * \param start_stop_flag switch if there will be a start and/or stop signal between the transmission
 * \return sucess or timeout
 */
uint8_t i2c_receive_burst(uint16_t base_address, uint8_t *p_rx_data, uint16_t bytes, uint8_t start_stop_flag);

/**
 * \fn i2c_receive
 *
 * \brief receive 1 byte
 * \param base_address is the I2C registers base address of the serial interface
 * \param start_stop_flag switch if there will be a start and/or stop signal between the transmission
 * \param rx_data is a pointer where to store the received data
 * \return sucess or timeout
 */
uint8_t i2c_receive(uint16_t base_address, uint8_t *rx_data,  uint8_t start_stop_flag);

#define     START_STOP   (0x0001)
#define     NO_START     (0x0002)
#define     NO_STOP      (0x0004)


#endif /* I2C_H_ */
