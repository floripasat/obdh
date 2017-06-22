#include "spi.h"

#define DUMMY 0xFF

void port_mapping_uca0(void) {
    // Enable Write-access to modify port mapping registers
    PMAPPWD = 0x02D52;

    // Allow reconfiguration during runtime
    PMAPCTL = PMAPRECFG;

    P2MAP0 = PM_UCA0CLK;
    P2MAP4 = PM_UCA0SIMO;
    P2MAP5 = PM_UCA0SOMI;

    // Disable Write-Access to modify port mapping registers
    PMAPPWD = 0;
}

void spi_setup(uint8_t interface) {

    switch(interface){
    case 0:
        BIT_SET(SPI0_SEL, SPI0_CLK_PIN | SPI0_MOSI_PIN | SPI0_MISO_PIN );
        spi_clock_setup(USCI_A0_BASE);
        port_mapping_uca0();
        break;
    case 1:
        BIT_SET(SPI1_SEL, SPI1_CLK_PIN | SPI1_MOSI_PIN | SPI1_MISO_PIN );
        spi_clock_setup(USCI_A1_BASE);
        break;
    }

}

void spi_clock_setup(uint16_t base_address) {
    HWREG8(base_address + OFS_UCAxCTL1) |= UCSWRST;                              // **Put state machine in reset**
//    HWREG8(base_address + OFS_UCAxCTL0) &= ~(UCCKPH + UCCKPL + UC7BIT + UCMSB +
//                                                UCMST + UCMODE_3 + UCSYNC);
    HWREG8(base_address + OFS_UCAxCTL0) |= UCMST | UCSYNC | UCCKPL | UCMSB;      // 3-pin, 8-bit SPI master
											                                    // Clock polarity high, MSB

	HWREG8(base_address + OFS_UCAxCTL1) |= UCSSEL_2;                             // SMCLK
	HWREG16(base_address + OFS_UCAxBRW) = 2;                                     // SMCLK/2 = 8MHz

	HWREG8(base_address + OFS_UCAxMCTL) = 0;                                     // No modulation
	HWREG8(base_address + OFS_UCAxCTL1) &= ~(UCSWRST);                           // **Initialize USCI state machine**
}
void wait_for_buffer_empty(uint16_t base_address){
    while (!(HWREG8(base_address + OFS_UCAxIFG) & UCTXIFG));
}
void wait_for_data_sent(uint16_t base_address){
    while (!(HWREG8(base_address + OFS_UCAxIFG) & UCRXIFG));
}

void spi_tx(uint16_t base_address, uint8_t data) {
	wait_for_buffer_empty(base_address);
	HWREG8(base_address + OFS_UCAxTXBUF) = data;
	wait_for_data_sent(base_address);
}

void spi_tx_multiple(uint16_t base_address, uint8_t *data,uint8_t data_bytes_length) {

	uint8_t i;

	for(i=0;i <= data_bytes_length;i++) {
		spi_tx(base_address, *data);
		data++;
	}
}

uint8_t spi_rx(uint16_t base_address) {

    wait_for_buffer_empty(base_address);
    HWREG8(base_address + OFS_UCAxTXBUF) = DUMMY;
    wait_for_data_sent(base_address);

	return (HWREG8(base_address + OFS_UCAxRXBUF));
}

void spi_rx_multiple(uint16_t base_address, uint8_t *data, uint8_t data_bytes_length) {

	uint8_t i;

	for(i = 0;i <= data_bytes_length; i++) {
		*data = spi_rx(base_address);
		data++;
    }

}


