#ifndef UTIL_SSPI_H_
#define UTIL_SSPI_H_

#include <msp430.h>
#include <stdint.h>
#include "../hal/obdh_hal.h"


void sspi_setup( void );
uint8_t sspi_tx (uint8_t data);
void sspi_tx_multiple(uint8_t *data,uint8_t data_bytes_length);

#define sspi_rx() sspi_tx(DUMMY);

void sspi_rx_multiple(uint8_t *data, uint8_t data_bytes_length);

#endif /* UTIL_SSPI_H_ */
