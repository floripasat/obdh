#ifndef UTIL_SSPI_H_
#define UTIL_SSPI_H_

#include <msp430.h>
#include <stdint.h>
#include "../hal/obdh_hal.h"

/**
 * \fn sspi_setup
 *
 * \brief Initialization of GPIOs: SCLK, MOSI, STE as output and MISO as input. STE disabled.
 * \return None
 */
void sspi_setup( void );

/**
 * \fn sspi_tx
 *
 * \brief Send a byte through the software SPI interface
 * \param byte_to_send is the 8-bit value that will be sent
 * \return the received byte of this transaction (full-duplex)
 */
uint8_t sspi_tx (uint8_t byte_to_send);

/**
 * \fn sspi_tx_multiple
 *
 * \brief Send one or more bytes through software SPI interface
 * \param data is a pointer to the first byte of the data to send
 * \param data_bytes_length is the length, in bytes, of the data to send
 * \return None
 */
void sspi_tx_multiple(uint8_t *data, uint8_t data_bytes_length);

/**
 * \fn sspi_rx
 *
 * \brief Receive a byte through the software SPI interface, sending a dummy byte (0xFF) to the slave
 * \return the received byte
 */
#define sspi_rx() sspi_tx(0xFF);

/**
 * \fn sspi_rx_multiple
 *
 * \brief Receive one or more bytes through software SPI interface
 * \param data is a pointer to the first byte of the received buffer
 * \param data_bytes_length is the length, in bytes, of the data to be received
 * \return None
 */
void sspi_rx_multiple(uint8_t *data, uint8_t data_bytes_length);

#endif /* UTIL_SSPI_H_ */
