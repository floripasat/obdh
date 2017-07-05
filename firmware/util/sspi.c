#include "sspi.h"

#define DUMMY 0xFF

void sspi_setup( void ) {

    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);
    BIT_CLEAR(TTC_MOSI_OUT, TTC_MOSI_PIN);

    BIT_SET(TTC_CLK_DIR, TTC_CLK_PIN);
    BIT_SET(TTC_MOSI_DIR, TTC_MOSI_PIN);
}

uint8_t sspi_send_bit(uint8_t tx_bit) {
    uint8_t rx_bit;

    BIT_PUT(TTC_MOSI_OUT, TTC_MOSI_PIN, tx_bit);        /**< MOSI = tx_bit  */
    BIT_SET(TTC_CLK_OUT, TTC_CLK_PIN);                  /**< Rise CLK       */
    rx_bit = BIT_READ(TTC_MISO_IN, TTC_MISO_PIN);       /**< rx_bit = MISO  */
    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);                /**< Fall CLK       */

    return rx_bit;
}

uint8_t sspi_tx (uint8_t data) {
    uint8_t received = 0;
    /*uint8_t counter;

    for (counter = 0; counter < 8; counter++) {
        received <<= 1;
        received |= sspi_send_bit(data>>7);
        data <<= 1;
    }*/


    BIT_PUT(TTC_MOSI_OUT, TTC_MOSI_PIN, data>>7);        /**< MOSI = tx_bit  */
    BIT_SET(TTC_CLK_OUT, TTC_CLK_PIN);                   /**< Rise CLK       */
    received |= BIT_READ(TTC_MISO_IN, TTC_MISO_PIN)<<7;  /**< rx_bit = MISO  */
    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);

    BIT_PUT(TTC_MOSI_OUT, TTC_MOSI_PIN, data>>6);        /**< MOSI = tx_bit  */
    BIT_SET(TTC_CLK_OUT, TTC_CLK_PIN);                   /**< Rise CLK       */
    received |= BIT_READ(TTC_MISO_IN, TTC_MISO_PIN)<<6;  /**< rx_bit = MISO  */
    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);

    BIT_PUT(TTC_MOSI_OUT, TTC_MOSI_PIN, data>>5);        /**< MOSI = tx_bit  */
    BIT_SET(TTC_CLK_OUT, TTC_CLK_PIN);                   /**< Rise CLK       */
    received |= BIT_READ(TTC_MISO_IN, TTC_MISO_PIN)<<5;  /**< rx_bit = MISO  */
    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);

    BIT_PUT(TTC_MOSI_OUT, TTC_MOSI_PIN, data>>4);        /**< MOSI = tx_bit  */
    BIT_SET(TTC_CLK_OUT, TTC_CLK_PIN);                   /**< Rise CLK       */
    received |= BIT_READ(TTC_MISO_IN, TTC_MISO_PIN)<<4;  /**< rx_bit = MISO  */
    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);

    BIT_PUT(TTC_MOSI_OUT, TTC_MOSI_PIN, data>>3);        /**< MOSI = tx_bit  */
    BIT_SET(TTC_CLK_OUT, TTC_CLK_PIN);                   /**< Rise CLK       */
    received |= BIT_READ(TTC_MISO_IN, TTC_MISO_PIN)<<3;  /**< rx_bit = MISO  */
    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);

    BIT_PUT(TTC_MOSI_OUT, TTC_MOSI_PIN, data>>2);        /**< MOSI = tx_bit  */
    BIT_SET(TTC_CLK_OUT, TTC_CLK_PIN);                   /**< Rise CLK       */
    received |= BIT_READ(TTC_MISO_IN, TTC_MISO_PIN)<<2;  /**< rx_bit = MISO  */
    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);

    BIT_PUT(TTC_MOSI_OUT, TTC_MOSI_PIN, data>>1);        /**< MOSI = tx_bit  */
    BIT_SET(TTC_CLK_OUT, TTC_CLK_PIN);                   /**< Rise CLK       */
    received |= BIT_READ(TTC_MISO_IN, TTC_MISO_PIN)<<1;  /**< rx_bit = MISO  */
    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);

    BIT_PUT(TTC_MOSI_OUT, TTC_MOSI_PIN, data>>0);        /**< MOSI = tx_bit  */
    BIT_SET(TTC_CLK_OUT, TTC_CLK_PIN);                   /**< Rise CLK       */
    received |= BIT_READ(TTC_MISO_IN, TTC_MISO_PIN)<<0;  /**< rx_bit = MISO  */
    BIT_CLEAR(TTC_CLK_OUT, TTC_CLK_PIN);


    return received;
}

void sspi_tx_multiple(uint8_t *data, uint8_t data_bytes_length) {

	uint8_t i;

	for(i=0;i <= data_bytes_length;i++) {
	    sspi_tx(*data);
		data++;
	}
}

void sspi_rx_multiple(uint8_t *data, uint8_t data_bytes_length) {

	uint8_t i;

	for(i = 0;i <= data_bytes_length; i++) {
		*data = sspi_rx();
		data++;
    }

}
