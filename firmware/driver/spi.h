#include <msp430.h>
#include <stdint.h>
#include "../hal/obdh_hal.h"



#define DUMMY 0x00

void spi_setup(uint8_t interface);
void spi_clock_setup(uint16_t base_address);
void spi_tx(uint16_t base_address, uint8_t data);
void spi_tx_multiple(uint16_t base_address, uint8_t *data,uint8_t data_bytes_length);
uint8_t spi_rx(uint16_t base_address);
void spi_rx_multiple(uint16_t base_address, uint8_t *data, uint8_t data_bytes_length);
