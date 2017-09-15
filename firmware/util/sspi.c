/*
 * sspi.c
 *
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
 *
 * This file is part of FloripaSat-OBDH.
 *
 * FloripaSat-OBDH is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FloripaSat-OBDH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /**
 * \file sspi.c
 *
 * \brief This file has the functions to use an SPI interface built in software
 *
 * \author Elder Tramontin
 *
 */

#include "sspi.h"

#define DUMMY                   0xFF
#define SLAVE_ENABLE()          BIT_CLEAR(TTC_STE_OUT, TTC_STE_PIN)     /**< slave select (active low) = 0 */
#define SLAVE_DISABLE()         BIT_SET(TTC_STE_OUT, TTC_STE_PIN)       /**< slave select (active low) = 1 */

#define TIME_TO_STEADY_OUTPUT   160                                    /**< cycles until the data is stable to be read */

void sspi_setup( void ) {

    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);        /**< slave clock = 0 */
    BIT_CLEAR(TTC_MOSI_OUT, TTC_MOSI_PIN);      /**< MOSI = 0 */
    SLAVE_DISABLE();

    /*
     * set SCLK, MOSI, STE as output and MISO as input
     */
    BIT_SET(TTC_STE_DIR, TTC_STE_PIN);
    BIT_SET(TTC_CLK_DIR, TTC_CLK_PIN);
    BIT_SET(TTC_MOSI_DIR, TTC_MOSI_PIN);
    BIT_CLEAR(TTC_MISO_DIR, TTC_MISO_PIN);
}

uint8_t sspi_send_bit(uint8_t tx_bit) { //CPOL = 0 CPHA = 0
    uint8_t rx_bit;

    __delay_cycles(TIME_TO_STEADY_OUTPUT);              /**< wait the MOSI be steady    */
    BIT_SET(TTC_CLK_OUT, TTC_CLK_PIN);                  /**< Rise CLK                   */
    BIT_PUT(TTC_MOSI_OUT, TTC_MOSI_PIN, tx_bit);        /**< MOSI = tx_bit              */
    __delay_cycles(TIME_TO_STEADY_OUTPUT);              /**< wait the CLK be steady     */
    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);                /**< Fall CLK                   */
    rx_bit = BIT_READ(TTC_MISO_IN, TTC_MISO_PIN);       /**< rx_bit = MISO              */

    return rx_bit;
}

uint8_t sspi_tx (uint8_t byte_to_send) {
    uint8_t byte_received = 0;
    int8_t counter;

    SLAVE_ENABLE();

    for (counter = 7; counter >= 0; counter--) {
        byte_received <<= 1;                                    /**< shift left the previous bits */
        byte_received |= sspi_send_bit(byte_to_send>>counter);  /**< send and receive a bit, according the current "counter" value, MSB first*/
    }

    SLAVE_DISABLE();

    return byte_received;
}

void sspi_tx_multiple(uint8_t *data, uint8_t data_bytes_length) {

	uint8_t i;

	for(i = 0; i < data_bytes_length; i++) {
	    sspi_tx(*data);                         /**< send a byte     */
		data++;                                 /**< go to next byte */
	}
}

void sspi_rx_multiple(uint8_t *data, uint8_t data_bytes_length) {

	uint8_t i;

	for(i = 0; i < data_bytes_length; i++) {
		*data = sspi_rx();                      /**< receive a byte  */
		data++;                                 /**< go to next byte */
    }
}
