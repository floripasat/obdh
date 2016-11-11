/*
 * radio.c
 *
 *  Created on: 11 de mai de 2016
 *      Author: mario
 */
#include <interface/radio.h>

void readTransceiver(char* buffer){

	 runRX(buffer);

}

void radio_Setup(void){

	registerConfig();
	manualCalibration();
	// Set radio in RX
	trxSpiCmdStrobe(CC112X_SRX);

}

void SPI_Setup(void){
    trxRfSpiInterfaceInit(2);				// ----> Configura USCI_A0 utilizado no FloripaSat
}

static void registerConfig(void) {

	uint16_t i;
    uint8_t writeByte;

    trxSpiCmdStrobe(CC112X_SRES);													// Reset radio

    for(i = 0; i < (sizeof(preferredSettings)/sizeof(registerSetting_t)); i++) {	// Write registers to radio
        	writeByte = preferredSettings[i].data;
        	cc112xSpiWriteReg(preferredSettings[i].addr, &writeByte, 1);
        }



}

/*******************************************************************************
*   @fn         runRX
*
*   @brief      Puts radio in RX and waits for packets. Function assumes
*               that status bytes are appended in the RX_FIFO
*               Update packet counter and display for each packet received.
*
*   @param      none
*
*   @return     none
*/
static void runRX(char* buffer) {

//	debug("runRX() init");
	uint8_t rxBuffer[150] = { 0 };
	uint8_t rxBytes;
	uint8_t marcState;


	// Set radio in RX
//	debug("trxSpiCmdStrobe() init");
	trxSpiCmdStrobe(CC112X_SRX);

	// Read number of bytes in RX FIFO
//	debug("cc112xSpiReadReg() init");
	cc112xSpiReadReg(CC112X_NUM_RXBYTES, &rxBytes, 1);
//	debug_uint("rxBytes:", rxBytes);

	// Check that we have bytes in FIFO
	if (rxBytes != 0) {

//		debug_uint("\n\n\rrxBytes:", rxBytes);

		// Read MARCSTATE to check for RX FIFO error
		// ~20ms
		cc112xSpiReadReg(CC112X_MARCSTATE, &marcState, 1);

		// Mask out MARCSTATE bits and check if we have a RX FIFO error
		if ((marcState & 0x1F) == RX_FIFO_ERROR) {
//			Flush RX FIFO
//			trxSpiCmdStrobe(CC112X_SFRX);

		} else {
//			debug("(marcState & 0x1F) == RX_FIFO_ERROR  ELSE");
//			// Read n bytes from RX FIFO

			cc112xSpiReadRxFifo(rxBuffer, rxBytes);

			//			debug("Printing radio buffer...");
			debug_array("\tRadio data:", rxBuffer, rxBytes );
//			debug_array_ascii("Radio ASCII:", rxBuffer, rxBytes );
			buffer[0] = rxBuffer[1];
			buffer[1] = rxBuffer[2];
			buffer[2] = rxBuffer[6];
			buffer[3] = rxBuffer[7];
		}

	} else {
		// even if there's no data to read, wait for a specific time
		// to make radio reading functions timming static.
	    __delay_cycles(DELAY_50_MS_IN_CYCLES);
	    __delay_cycles(DELAY_10_MS_IN_CYCLES);
	    __delay_cycles(DELAY_1_MS_IN_CYCLES);
	}

	// Set radio back in RX
	trxSpiCmdStrobe(CC112X_SRX);
}

void trxRfSpiInterfaceInit(uint8_t prescalerValue){
	/* SPI is configured in USCI_A0	 */

	/* Keep peripheral in reset state*/
	UCA0CTL1 |= UCSWRST;			// UCSWRST (0x01) USCI Software Reset (1 Enabled. USCI logic held in reset state)

	// Testando o P2MAP
	P2MAP0 = PM_UCA0CLK;
	P2MAP4 = PM_UCA0SIMO;
	P2MAP5 = PM_UCA0SOMI;

	/* Configuration
	 * -  8-bit
	 * -  Master Mode
	 * -  3-pin
	 * -  synchronous mode
	 * -  MSB first
	 * -  Clock phase select = captured on first edge
	 * -  Inactive state is low
	 * -  SMCLK as clock source
	 * -  Spi clk is adjusted corresponding to systemClock as the highest rate
	 *    supported by the supported radios: this could be optimized and done
	 *    after chip detect.
	*/
	UCA0CTL0  =  0x00+UCMST + UCSYNC + UCMODE_0 + UCMSB + UCCKPH;
	UCA0CTL1 |=  UCSSEL_2;							// USCI clock source select. SCLK do SPI trabalha com SMCLK
	UCA0BR1   =  0x00;
    UCA0BR0   = prescalerValue;


	/* Configure port and pins
	 * - MISO/MOSI/SCLK GPIO controlled by peripheral
	 * - CS_n GPIO controlled manually, set to 1
	*/
	// P2SEL       |= BIT4 (MOSI)		 + BIT5 (MISO)		  + BIT0 (SCLK)
	TRXEM_PORT_SEL |= TRXEM_SPI_MOSI_PIN + TRXEM_SPI_MISO_PIN + TRXEM_SPI_SCLK_PIN;

	// P2SEL       |= BIT3 (CSn)
	TRXEM_PORT_SEL &= ~TRXEM_SPI_SC_N_PIN;	// P2.3 (CSn)
	TRXEM_PORT_OUT |= TRXEM_SPI_SC_N_PIN + TRXEM_SPI_MISO_PIN;/* Pullup on MISO */		// Original

	TRXEM_PORT_DIR |= TRXEM_SPI_SC_N_PIN;
	/* In case not automatically set */
	TRXEM_PORT_DIR |= TRXEM_SPI_MOSI_PIN + TRXEM_SPI_SCLK_PIN;
	TRXEM_PORT_DIR &= ~TRXEM_SPI_MISO_PIN;

	// P1.3 (p 37) - RESETn_TRANS
	P1DIR |= BIT3; 	// set as output
	P1OUT |= BIT3;	// set 1

	// RF SPI0 CS as GPIO output high P2.3 	(CSn FloripaSat)
	P2SEL &= ~BIT3;
	P2OUT |=  BIT3;
	P2DIR |=  BIT3;



	/* Release for operation */
	UCA0CTL1 &= ~UCSWRST;
	return;
}

/*******************************************************************************
*   @fn         manualCalibration
*   @brief      Calibrates radio according to CC112x errata
*   @param      none
*   @return     none
*
*   [Julian]:	The ManualCalibration is suggested in "CC112x,CC1175 Silicon Errata" <http://www.ti.com/lit/er/swrz039d/swrz039d.pdf>
*/
#define VCDAC_START_OFFSET 2
#define FS_VCO2_INDEX 0
#define FS_VCO4_INDEX 1
#define FS_CHP_INDEX 2
static void manualCalibration(void) {



    uint8_t original_fs_cal2;
    uint8_t calResults_for_vcdac_start_high[3];
    uint8_t calResults_for_vcdac_start_mid[3];
    uint8_t marcstate;
    uint8_t writeByte;

    // 1) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
    writeByte = 0x00;
    cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);

    // 2) Start with high VCDAC (original VCDAC_START + 2):
    cc112xSpiReadReg(CC112X_FS_CAL2, &original_fs_cal2, 1);
    writeByte = original_fs_cal2 + VCDAC_START_OFFSET;
    cc112xSpiWriteReg(CC112X_FS_CAL2, &writeByte, 1);

    // 3) Calibrate and wait for calibration to be done
    //   (radio back in IDLE state)
    trxSpiCmdStrobe(CC112X_SCAL);

    do {																				// <--------- fica nesse loop
        cc112xSpiReadReg(CC112X_MARCSTATE, &marcstate, 1);
    } while (marcstate != 0x41);

    // 4) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained with
    //    high VCDAC_START value
    cc112xSpiReadReg(CC112X_FS_VCO2,
                     &calResults_for_vcdac_start_high[FS_VCO2_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_VCO4,
                     &calResults_for_vcdac_start_high[FS_VCO4_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_CHP,
                     &calResults_for_vcdac_start_high[FS_CHP_INDEX], 1);

    // 5) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
    writeByte = 0x00;
    cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);

    // 6) Continue with mid VCDAC (original VCDAC_START):
    writeByte = original_fs_cal2;
    cc112xSpiWriteReg(CC112X_FS_CAL2, &writeByte, 1);

    // 7) Calibrate and wait for calibration to be done
    //   (radio back in IDLE state)
    trxSpiCmdStrobe(CC112X_SCAL);

    do {
        cc112xSpiReadReg(CC112X_MARCSTATE, &marcstate, 1);
    } while (marcstate != 0x41);

    // 8) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained
    //    with mid VCDAC_START value
    cc112xSpiReadReg(CC112X_FS_VCO2,
                     &calResults_for_vcdac_start_mid[FS_VCO2_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_VCO4,
                     &calResults_for_vcdac_start_mid[FS_VCO4_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_CHP,
                     &calResults_for_vcdac_start_mid[FS_CHP_INDEX], 1);

    // 9) Write back highest FS_VCO2 and corresponding FS_VCO
    //    and FS_CHP result
    if (calResults_for_vcdac_start_high[FS_VCO2_INDEX] >
        calResults_for_vcdac_start_mid[FS_VCO2_INDEX]) {
        writeByte = calResults_for_vcdac_start_high[FS_VCO2_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_high[FS_VCO4_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO4, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_high[FS_CHP_INDEX];
        cc112xSpiWriteReg(CC112X_FS_CHP, &writeByte, 1);
    } else {
        writeByte = calResults_for_vcdac_start_mid[FS_VCO2_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_mid[FS_VCO4_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO4, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_mid[FS_CHP_INDEX];
        cc112xSpiWriteReg(CC112X_FS_CHP, &writeByte, 1);
    }


}
