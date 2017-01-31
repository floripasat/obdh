/*
 * radio.h
 *
 *  Created on: 11 de mai de 2016
 *      Author: mario
 */
#ifndef INTERFACE_RADIO_H_
#define INTERFACE_RADIO_H_

#include <msp430.h>
#include <stdint.h>
#include "../hal/obdh_hal.h"
#include "../util/debug.h"
#include "../util/misc.h"


//#include "../util/spi.h"

#define st(x)      do { x } while (__LINE__ == -1)
#define NOP()  asm(" nop")


/******************************************************************************
* DEFINES
*/

#define RX_FIFO_ERROR           0x11
#define PKTLEN                  30  		// 1 < PKTLEN < 126
#define GPIO3                   0x80		// P2.7 (1000 0000)	FloripaSat
#define GPIO2                   0x20		// P1.5 (0010 0000) FloripaSat
#define GPIO0                   0x40		// P1.6 (0100 0000) FloripaSat

/*******************************************************************************
* LOCAL VARIABLES
*/

//uint16_t packetCounter = 0;

uint8_t saveRegisterTest;
uint8_t leituraFIFO;
uint8_t leituratrans;



/******************************************************************************
 * CONSTANTS
 */

// MCU MSP430F6659 FloripaSat
#define TRXEM_PORT_SEL       P2SEL
#define TRXEM_PORT_OUT       P2OUT
#define TRXEM_PORT_DIR       P2DIR
#define TRXEM_PORT_IN        P2IN

// MCU MSP430F6659 FloripaSat
#define TRXEM_SPI_MOSI_PIN   BIT4		// P2.4
#define TRXEM_SPI_MISO_PIN   BIT5		// P2.5
#define TRXEM_SPI_SCLK_PIN   BIT0		// P2.0
#define TRXEM_SPI_SC_N_PIN   BIT3		// P2.3

// MCU MSP430F6659 FloripaSat
#define RF_RESET_N_PORT_OUT  P1OUT
#define RF_RESET_N_PORT_SEL  P1SEL
#define RF_RESET_N_PORT_DIR  P1DIR
#define RF_RESET_N_PIN       BIT3		// P1.3 (RESE_n transceiver)


#define RADIO_BURST_ACCESS   0x40		// [R/W' 1 A5 A4 A3 A2 A1 A0] = 0100 0000 = 0x40 (CC112x User's Guide pg.10)
#define RADIO_SINGLE_ACCESS  0x00		// [R/W' 0 A5 A4 A3 A2 A1 A0] = 0000 0000 = 0x00 (CC112x User's Guide pg.10)
#define RADIO_READ_ACCESS    0x80		// [1 B/S A5 A4 A3 A2 A1 A0]  = 1000 0000 = 0x80 (CC112x User's Guide pg.10)
#define RADIO_WRITE_ACCESS   0x00		// [0 B/S A5 A4 A3 A2 A1 A0]  = 0000 0000 = 0x00 (CC112x User's Guide pg.10)


/* Bit fields in the chip status byte */
												// [x7 y6 y5 y4 z3 z2 z1 z0]  (CC112x User's Guide pg.9)
#define STATUS_CHIP_RDYn_BM             0x80 	// x (default 1b): Stays high until power and crystal have stabilized. Should always be low when using the SPI interface.
#define STATUS_STATE_BM                 0x70	// y (default 111b): Indicates the current main state machine mode (State default TX FIFO ERROR)
#define STATUS_FIFO_BYTES_AVAILABLE_BM  0x0F	// z (default 1111b): Reserved


/******************************************************************************
 * MACROS
 */

/* Macros for Tranceivers(TRX) */
#define TRXEM_SPI_BEGIN()              st( TRXEM_PORT_OUT &= ~TRXEM_SPI_SC_N_PIN; NOP(); )
#define TRXEM_SPI_TX(x)                st( UCA0IFG &= ~UCRXIFG; UCA0TXBUF= (x); )
#define TRXEM_SPI_WAIT_DONE()          st( while(!(UCA0IFG & UCRXIFG)); )
#define TRXEM_SPI_RX()                 UCA0RXBUF
#define TRXEM_SPI_WAIT_MISO_LOW(x)     st( uint8_t count = 200; \
                                           while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN) \
                                           { \
                                              __delay_cycles(5000); \
                                              count--; \
                                              if (count == 0) break; \
                                           } \
                                           if(count>0) (x) = 1; \
                                           else (x) = 0; )

#define TRXEM_SPI_END()                st( NOP(); TRXEM_PORT_OUT |= TRXEM_SPI_SC_N_PIN; )

/******************************************************************************
 * TYPEDEFS
 */

typedef struct
{
  uint16_t  addr;
  uint8_t   data;
}registerSetting_t;

typedef uint8_t rfStatus_t;

/******************************************************************************
 * PROTOTYPES
 */

void trxRfSpiInterfaceInit(uint8_t clockDivider);
rfStatus_t trx8BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint16_t len);
rfStatus_t trxSpiCmdStrobe(uint8_t cmd);
static void trxReadWriteBurstSingle(uint8_t addr,uint8_t *pData,uint16_t len) ;


/* CC112X specific prototype function */
rfStatus_t trx16BitRegAccess(uint8_t accessType, uint8_t extAddr, uint8_t regAddr, uint8_t *pData, uint8_t len);



/******************************************************************************
 * CONSTANTS
 */

/* configuration registers */
#define CC112X_IOCFG3                   0x0000
#define CC112X_IOCFG2                   0x0001
#define CC112X_IOCFG1                   0x0002
#define CC112X_IOCFG0                   0x0003
#define CC112X_SYNC3                    0x0004
#define CC112X_SYNC2                    0x0005
#define CC112X_SYNC1                    0x0006
#define CC112X_SYNC0                    0x0007
#define CC112X_SYNC_CFG1                0x0008
#define CC112X_SYNC_CFG0                0x0009
#define CC112X_DEVIATION_M              0x000A
#define CC112X_MODCFG_DEV_E             0x000B
#define CC112X_DCFILT_CFG               0x000C
#define CC112X_PREAMBLE_CFG1            0x000D
#define CC112X_PREAMBLE_CFG0            0x000E
#define CC112X_FREQ_IF_CFG              0x000F
#define CC112X_IQIC                     0x0010
#define CC112X_CHAN_BW                  0x0011
#define CC112X_MDMCFG1                  0x0012
#define CC112X_MDMCFG0                  0x0013
#define CC112X_SYMBOL_RATE2             0x0014
#define CC112X_SYMBOL_RATE1             0x0015
#define CC112X_SYMBOL_RATE0             0x0016
#define CC112X_AGC_REF                  0x0017
#define CC112X_AGC_CS_THR               0x0018
#define CC112X_AGC_GAIN_ADJUST          0x0019
#define CC112X_AGC_CFG3                 0x001A
#define CC112X_AGC_CFG2                 0x001B
#define CC112X_AGC_CFG1                 0x001C
#define CC112X_AGC_CFG0                 0x001D
#define CC112X_FIFO_CFG                 0x001E
#define CC112X_DEV_ADDR                 0x001F
#define CC112X_SETTLING_CFG             0x0020
#define CC112X_FS_CFG                   0x0021
#define CC112X_WOR_CFG1                 0x0022
#define CC112X_WOR_CFG0                 0x0023
#define CC112X_WOR_EVENT0_MSB           0x0024
#define CC112X_WOR_EVENT0_LSB           0x0025
#define CC112X_PKT_CFG2                 0x0026
#define CC112X_PKT_CFG1                 0x0027
#define CC112X_PKT_CFG0                 0x0028
#define CC112X_RFEND_CFG1               0x0029
#define CC112X_RFEND_CFG0               0x002A
#define CC112X_PA_CFG2                  0x002B
#define CC112X_PA_CFG1                  0x002C
#define CC112X_PA_CFG0                  0x002D
#define CC112X_PKT_LEN                  0x002E

/* Extended Configuration Registers */
#define CC112X_IF_MIX_CFG               0x2F00
#define CC112X_FREQOFF_CFG              0x2F01
#define CC112X_TOC_CFG                  0x2F02
#define CC112X_MARC_SPARE               0x2F03
#define CC112X_ECG_CFG                  0x2F04
#define CC112X_CFM_DATA_CFG             0x2F05
#define CC112X_EXT_CTRL                 0x2F06
#define CC112X_RCCAL_FINE               0x2F07
#define CC112X_RCCAL_COARSE             0x2F08
#define CC112X_RCCAL_OFFSET             0x2F09
#define CC112X_FREQOFF1                 0x2F0A
#define CC112X_FREQOFF0                 0x2F0B
#define CC112X_FREQ2                    0x2F0C
#define CC112X_FREQ1                    0x2F0D
#define CC112X_FREQ0                    0x2F0E
#define CC112X_IF_ADC2                  0x2F0F
#define CC112X_IF_ADC1                  0x2F10
#define CC112X_IF_ADC0                  0x2F11
#define CC112X_FS_DIG1                  0x2F12
#define CC112X_FS_DIG0                  0x2F13
#define CC112X_FS_CAL3                  0x2F14
#define CC112X_FS_CAL2                  0x2F15
#define CC112X_FS_CAL1                  0x2F16
#define CC112X_FS_CAL0                  0x2F17
#define CC112X_FS_CHP                   0x2F18
#define CC112X_FS_DIVTWO                0x2F19
#define CC112X_FS_DSM1                  0x2F1A
#define CC112X_FS_DSM0                  0x2F1B
#define CC112X_FS_DVC1                  0x2F1C
#define CC112X_FS_DVC0                  0x2F1D
#define CC112X_FS_LBI                   0x2F1E
#define CC112X_FS_PFD                   0x2F1F
#define CC112X_FS_PRE                   0x2F20
#define CC112X_FS_REG_DIV_CML           0x2F21
#define CC112X_FS_SPARE                 0x2F22
#define CC112X_FS_VCO4                  0x2F23
#define CC112X_FS_VCO3                  0x2F24
#define CC112X_FS_VCO2                  0x2F25
#define CC112X_FS_VCO1                  0x2F26
#define CC112X_FS_VCO0                  0x2F27
#define CC112X_GBIAS6                   0x2F28
#define CC112X_GBIAS5                   0x2F29
#define CC112X_GBIAS4                   0x2F2A
#define CC112X_GBIAS3                   0x2F2B
#define CC112X_GBIAS2                   0x2F2C
#define CC112X_GBIAS1                   0x2F2D
#define CC112X_GBIAS0                   0x2F2E
#define CC112X_IFAMP                    0x2F2F
#define CC112X_LNA                      0x2F30
#define CC112X_RXMIX                    0x2F31
#define CC112X_XOSC5                    0x2F32
#define CC112X_XOSC4                    0x2F33
#define CC112X_XOSC3                    0x2F34
#define CC112X_XOSC2                    0x2F35
#define CC112X_XOSC1                    0x2F36
#define CC112X_XOSC0                    0x2F37
#define CC112X_ANALOG_SPARE             0x2F38
#define CC112X_PA_CFG3                  0x2F39
#define CC112X_IRQ0M                    0x2F3F
#define CC112X_IRQ0F                    0x2F40

/* Status Registers */
#define CC112X_WOR_TIME1                0x2F64
#define CC112X_WOR_TIME0                0x2F65
#define CC112X_WOR_CAPTURE1             0x2F66
#define CC112X_WOR_CAPTURE0             0x2F67
#define CC112X_BIST                     0x2F68
#define CC112X_DCFILTOFFSET_I1          0x2F69
#define CC112X_DCFILTOFFSET_I0          0x2F6A
#define CC112X_DCFILTOFFSET_Q1          0x2F6B
#define CC112X_DCFILTOFFSET_Q0          0x2F6C
#define CC112X_IQIE_I1                  0x2F6D
#define CC112X_IQIE_I0                  0x2F6E
#define CC112X_IQIE_Q1                  0x2F6F
#define CC112X_IQIE_Q0                  0x2F70
#define CC112X_RSSI1                    0x2F71
#define CC112X_RSSI0                    0x2F72
#define CC112X_MARCSTATE                0x2F73
#define CC112X_LQI_VAL                  0x2F74
#define CC112X_PQT_SYNC_ERR             0x2F75
#define CC112X_DEM_STATUS               0x2F76
#define CC112X_FREQOFF_EST1             0x2F77
#define CC112X_FREQOFF_EST0             0x2F78
#define CC112X_AGC_GAIN3                0x2F79
#define CC112X_AGC_GAIN2                0x2F7A
#define CC112X_AGC_GAIN1                0x2F7B
#define CC112X_AGC_GAIN0                0x2F7C
#define CC112X_CFM_RX_DATA_OUT          0x2F7D
#define CC112X_CFM_TX_DATA_IN           0x2F7E
#define CC112X_ASK_SOFT_RX_DATA         0x2F7F
#define CC112X_RNDGEN                   0x2F80
#define CC112X_MAGN2                    0x2F81
#define CC112X_MAGN1                    0x2F82
#define CC112X_MAGN0                    0x2F83
#define CC112X_ANG1                     0x2F84
#define CC112X_ANG0                     0x2F85
#define CC112X_CHFILT_I2                0x2F86
#define CC112X_CHFILT_I1                0x2F87
#define CC112X_CHFILT_I0                0x2F88
#define CC112X_CHFILT_Q2                0x2F89
#define CC112X_CHFILT_Q1                0x2F8A
#define CC112X_CHFILT_Q0                0x2F8B
#define CC112X_GPIO_STATUS              0x2F8C
#define CC112X_FSCAL_CTRL               0x2F8D
#define CC112X_PHASE_ADJUST             0x2F8E
#define CC112X_PARTNUMBER               0x2F8F
#define CC112X_PARTVERSION              0x2F90
#define CC112X_SERIAL_STATUS            0x2F91
#define CC112X_MODEM_STATUS1            0x2F92
#define CC112X_MODEM_STATUS0            0x2F93
#define CC112X_MARC_STATUS1             0x2F94
#define CC112X_MARC_STATUS0             0x2F95
#define CC112X_PA_IFAMP_TEST            0x2F96
#define CC112X_FSRF_TEST                0x2F97
#define CC112X_PRE_TEST                 0x2F98
#define CC112X_PRE_OVR                  0x2F99
#define CC112X_ADC_TEST                 0x2F9A
#define CC112X_DVC_TEST                 0x2F9B
#define CC112X_ATEST                    0x2F9C
#define CC112X_ATEST_LVDS               0x2F9D
#define CC112X_ATEST_MODE               0x2F9E
#define CC112X_XOSC_TEST1               0x2F9F
#define CC112X_XOSC_TEST0               0x2FA0

#define CC112X_RXFIRST                  0x2FD2
#define CC112X_TXFIRST                  0x2FD3
#define CC112X_RXLAST                   0x2FD4
#define CC112X_TXLAST                   0x2FD5
#define CC112X_NUM_TXBYTES              0x2FD6  /* Number of bytes in TXFIFO */
#define CC112X_NUM_RXBYTES              0x2FD7  /* Number of bytes in RXFIFO */
#define CC112X_FIFO_NUM_TXBYTES         0x2FD8
#define CC112X_FIFO_NUM_RXBYTES         0x2FD9


/* DATA FIFO Access */
#define CC112X_SINGLE_TXFIFO            0x003F      /*  TXFIFO  - Single accecss to Transmit FIFO */
#define CC112X_BURST_TXFIFO             0x007F      /*  TXFIFO  - Burst accecss to Transmit FIFO  */
#define CC112X_SINGLE_RXFIFO            0x00BF      /*  RXFIFO  - Single accecss to Receive FIFO  */
#define CC112X_BURST_RXFIFO             0x00FF      /*  RXFIFO  - Busrrst ccecss to Receive FIFO  */

#define CC112X_LQI_CRC_OK_BM            0x80
#define CC112X_LQI_EST_BM               0x7F



/* Command strobe registers */
#define CC112X_SRES                     0x30      /*  SRES    - Reset chip. */
#define CC112X_SFSTXON                  0x31      /*  SFSTXON - Enable and calibrate frequency synthesizer. */
#define CC112X_SXOFF                    0x32      /*  SXOFF   - Turn off crystal oscillator. */
#define CC112X_SCAL                     0x33      /*  SCAL    - Calibrate frequency synthesizer and turn it off. */
#define CC112X_SRX                      0x34      /*  SRX     - Enable RX. Perform calibration if enabled. */
#define CC112X_STX                      0x35      /*  STX     - Enable TX. If in RX state, only enable TX if CCA passes. */
#define CC112X_SIDLE                    0x36      /*  SIDLE   - Exit RX / TX, turn off frequency synthesizer. */
#define CC112X_SWOR                     0x38      /*  SWOR    - Start automatic RX polling sequence (Wake-on-Radio) */
#define CC112X_SPWD                     0x39      /*  SPWD    - Enter power down mode when CSn goes high. */
#define CC112X_SFRX                     0x3A      /*  SFRX    - Flush the RX FIFO buffer. */
#define CC112X_SFTX                     0x3B      /*  SFTX    - Flush the TX FIFO buffer. */
#define CC112X_SWORRST                  0x3C      /*  SWORRST - Reset real time clock. */
#define CC112X_SNOP                     0x3D      /*  SNOP    - No operation. Returns status byte. */
#define CC112X_AFC                      0x37      /*  AFC     - Automatic Frequency Correction */

/* Chip states returned in status byte */
#define CC112X_STATE_IDLE               0x00
#define CC112X_STATE_RX                 0x10
#define CC112X_STATE_TX                 0x20
#define CC112X_STATE_FSTXON             0x30
#define CC112X_STATE_CALIBRATE          0x40
#define CC112X_STATE_SETTLING           0x50
#define CC112X_STATE_RXFIFO_ERROR       0x60
#define CC112X_STATE_TXFIFO_ERROR       0x70

/******************************************************************************
 * PROTPTYPES
 */

/* basic set of access functions */
rfStatus_t cc112xSpiReadReg(uint16_t addr, uint8_t *data, uint8_t len);
rfStatus_t cc112xGetTxStatus(void);
rfStatus_t cc112xGetRxStatus(void);
rfStatus_t cc112xSpiWriteReg(uint16_t addr, uint8_t *data, uint8_t len);
rfStatus_t cc112xSpiWriteTxFifo(uint8_t *pWriteData, uint8_t len);
rfStatus_t cc112xSpiReadRxFifo(uint8_t *pReadData, uint8_t len);
static void manualCalibration(void);


void readTransceiver(char*);
void SPI_Setup(void);
void radio_Setup(void);
static void registerConfig(void);
static void runRX(char*);
static void manualCalibration(void);

//todo new configuration of radio ** to be tested **
static const registerSetting_t preferredSettings[]=
{
    {CC112X_IOCFG3,              0xB0},
    {CC112X_IOCFG2,              0x06},
    {CC112X_IOCFG1,              0xB0},
    {CC112X_IOCFG0,              0x40},
    {CC112X_SYNC3,               0x93},
    {CC112X_SYNC2,               0x0B},
    {CC112X_SYNC1,               0x51},
    {CC112X_SYNC0,               0xDE},
    {CC112X_SYNC_CFG1,           0x0B},
    {CC112X_SYNC_CFG0,           0x17},
    {CC112X_DEVIATION_M,         0x06},
    {CC112X_MODCFG_DEV_E,        0x03},
    {CC112X_DCFILT_CFG,          0x1C},
    {CC112X_PREAMBLE_CFG1,       0x14},
    {CC112X_PREAMBLE_CFG0,       0x2A},
    {CC112X_FREQ_IF_CFG,         0x40},
    {CC112X_IQIC,                0xC6},
    {CC112X_CHAN_BW,             0x0A},
    {CC112X_MDMCFG1,             0x46},
    {CC112X_MDMCFG0,             0x05},
    {CC112X_SYMBOL_RATE2,        0x43},
    {CC112X_SYMBOL_RATE1,        0xA9},
    {CC112X_SYMBOL_RATE0,        0x2A},
    {CC112X_AGC_REF,             0x20},
    {CC112X_AGC_CS_THR,          0x19},
    {CC112X_AGC_GAIN_ADJUST,     0x00},
    {CC112X_AGC_CFG3,            0x91},
    {CC112X_AGC_CFG2,            0x20},
    {CC112X_AGC_CFG1,            0xA9},
    {CC112X_AGC_CFG0,            0xCF},
    {CC112X_FIFO_CFG,            0x00},
    {CC112X_DEV_ADDR,            0x00},
    {CC112X_SETTLING_CFG,        0x0B},
    {CC112X_FS_CFG,              0x14},
    {CC112X_WOR_CFG1,            0x08},
    {CC112X_WOR_CFG0,            0x21},
    {CC112X_WOR_EVENT0_MSB,      0x00},
    {CC112X_WOR_EVENT0_LSB,      0x00},
    {CC112X_PKT_CFG2,            0x04},
    {CC112X_PKT_CFG1,            0x05},
    {CC112X_PKT_CFG0,            0x20},
    {CC112X_RFEND_CFG1,          0x0F},
    {CC112X_RFEND_CFG0,          0x00},
    {CC112X_PA_CFG2,             0x7F},
    {CC112X_PA_CFG1,             0x56},
    {CC112X_PA_CFG0,             0x7E},
    {CC112X_PKT_LEN,             0xFF},
    {CC112X_IF_MIX_CFG,          0x00},
    {CC112X_FREQOFF_CFG,         0x22},
    {CC112X_TOC_CFG,             0x0B},
    {CC112X_MARC_SPARE,          0x00},
    {CC112X_ECG_CFG,             0x00},
    {CC112X_CFM_DATA_CFG,        0x00},
    {CC112X_EXT_CTRL,            0x01},
    {CC112X_RCCAL_FINE,          0x00},
    {CC112X_RCCAL_COARSE,        0x00},
    {CC112X_RCCAL_OFFSET,        0x00},
    {CC112X_FREQOFF1,            0x00},
    {CC112X_FREQOFF0,            0x00},
    {CC112X_FREQ2,               0x6C},
    {CC112X_FREQ1,               0x40},
    {CC112X_FREQ0,               0x00},
    {CC112X_IF_ADC2,             0x02},
    {CC112X_IF_ADC1,             0xA6},
    {CC112X_IF_ADC0,             0x04},
    {CC112X_FS_DIG1,             0x00},
    {CC112X_FS_DIG0,             0x5F},
    {CC112X_FS_CAL3,             0x00},
    {CC112X_FS_CAL2,             0x20},
    {CC112X_FS_CAL1,             0x00},
    {CC112X_FS_CAL0,             0x0E},
    {CC112X_FS_CHP,              0x28},
    {CC112X_FS_DIVTWO,           0x03},
    {CC112X_FS_DSM1,             0x00},
    {CC112X_FS_DSM0,             0x33},
    {CC112X_FS_DVC1,             0xFF},
    {CC112X_FS_DVC0,             0x17},
    {CC112X_FS_LBI,              0x00},
    {CC112X_FS_PFD,              0x50},
    {CC112X_FS_PRE,              0x6E},
    {CC112X_FS_REG_DIV_CML,      0x14},
    {CC112X_FS_SPARE,            0xAC},
    {CC112X_FS_VCO4,             0x14},
    {CC112X_FS_VCO3,             0x00},
    {CC112X_FS_VCO2,             0x00},
    {CC112X_FS_VCO1,             0x00},
    {CC112X_FS_VCO0,             0x81},
    {CC112X_GBIAS6,              0x00},
    {CC112X_GBIAS5,              0x02},
    {CC112X_GBIAS4,              0x00},
    {CC112X_GBIAS3,              0x00},
    {CC112X_GBIAS2,              0x10},
    {CC112X_GBIAS1,              0x00},
    {CC112X_GBIAS0,              0x00},
    {CC112X_IFAMP,               0x01},
    {CC112X_LNA,                 0x01},
    {CC112X_RXMIX,               0x01},
    {CC112X_XOSC5,               0x0E},
    {CC112X_XOSC4,               0xA0},
    {CC112X_XOSC3,               0xC7},
    {CC112X_XOSC2,               0x04},
    {CC112X_XOSC1,               0x07},
    {CC112X_XOSC0,               0x00},
    {CC112X_ANALOG_SPARE,        0x00},
    {CC112X_PA_CFG3,             0x00},
    {CC112X_WOR_TIME1,           0x00},
    {CC112X_WOR_TIME0,           0x00},
    {CC112X_WOR_CAPTURE1,        0x00},
    {CC112X_WOR_CAPTURE0,        0x00},
    {CC112X_BIST,                0x00},
    {CC112X_DCFILTOFFSET_I1,     0x00},
    {CC112X_DCFILTOFFSET_I0,     0x00},
    {CC112X_DCFILTOFFSET_Q1,     0x00},
    {CC112X_DCFILTOFFSET_Q0,     0x00},
    {CC112X_IQIE_I1,             0x00},
    {CC112X_IQIE_I0,             0x00},
    {CC112X_IQIE_Q1,             0x00},
    {CC112X_IQIE_Q0,             0x00},
    {CC112X_RSSI1,               0x80},
    {CC112X_RSSI0,               0x00},
    {CC112X_MARCSTATE,           0x41},
    {CC112X_LQI_VAL,             0x00},
    {CC112X_PQT_SYNC_ERR,        0xFF},
    {CC112X_DEM_STATUS,          0x00},
    {CC112X_FREQOFF_EST1,        0x00},
    {CC112X_FREQOFF_EST0,        0x00},
    {CC112X_AGC_GAIN3,           0x00},
    {CC112X_AGC_GAIN2,           0xD1},
    {CC112X_AGC_GAIN1,           0x00},
    {CC112X_AGC_GAIN0,           0x3F},
    {CC112X_CFM_RX_DATA_OUT,     0x00},
    {CC112X_CFM_TX_DATA_IN,      0x00},
    {CC112X_ASK_SOFT_RX_DATA,    0x30},
    {CC112X_RNDGEN,              0x7F},
    {CC112X_MAGN2,               0x00},
    {CC112X_MAGN1,               0x00},
    {CC112X_MAGN0,               0x00},
    {CC112X_ANG1,                0x00},
    {CC112X_ANG0,                0x00},
    {CC112X_CHFILT_I2,           0x08},
    {CC112X_CHFILT_I1,           0x00},
    {CC112X_CHFILT_I0,           0x00},
    {CC112X_CHFILT_Q2,           0x00},
    {CC112X_CHFILT_Q1,           0x00},
    {CC112X_CHFILT_Q0,           0x00},
    {CC112X_GPIO_STATUS,         0x00},
    {CC112X_FSCAL_CTRL,          0x01},
    {CC112X_PHASE_ADJUST,        0x00},
    {CC112X_PARTNUMBER,          0x58},
    {CC112X_PARTVERSION,         0x23},
    {CC112X_SERIAL_STATUS,       0x00},
    {CC112X_MODEM_STATUS1,       0x10},
    {CC112X_MODEM_STATUS0,       0x00},
    {CC112X_MARC_STATUS1,        0x00},
    {CC112X_MARC_STATUS0,        0x00},
    {CC112X_PA_IFAMP_TEST,       0x00},
    {CC112X_FSRF_TEST,           0x00},
    {CC112X_PRE_TEST,            0x00},
    {CC112X_PRE_OVR,             0x00},
    {CC112X_ADC_TEST,            0x00},
    {CC112X_DVC_TEST,            0x0B},
    {CC112X_ATEST,               0x40},
    {CC112X_ATEST_LVDS,          0x00},
    {CC112X_ATEST_MODE,          0x00},
    {CC112X_XOSC_TEST1,          0x3C},
    {CC112X_XOSC_TEST0,          0x00},
    {CC112X_RXFIRST,             0x00},
    {CC112X_TXFIRST,             0x00},
    {CC112X_RXLAST,              0x00},
    {CC112X_TXLAST,              0x00},
    {CC112X_NUM_TXBYTES,         0x00},
    {CC112X_NUM_RXBYTES,         0x00},
    {CC112X_FIFO_NUM_TXBYTES,    0x0F},
    {CC112X_FIFO_NUM_RXBYTES,    0x00},
};



//todo old congiguration of the radio

//static const registerSetting_t preferredSettings[]=
//{
//  {CC112X_IOCFG3,              0xB0},
//  {CC112X_IOCFG2,              0x06},
//  {CC112X_IOCFG1,              0xB0},
//  {CC112X_IOCFG0,              0x40},
//  {CC112X_SYNC3,               0x93},
//  {CC112X_SYNC2,               0x0B},
//  {CC112X_SYNC1,               0x51},
//  {CC112X_SYNC0,               0xDE},
//  {CC112X_SYNC_CFG1,           0x0B},
//  {CC112X_SYNC_CFG0,           0x17},
//  {CC112X_DEVIATION_M,         0x06},
//  {CC112X_MODCFG_DEV_E,        0x03},
//  {CC112X_DCFILT_CFG,          0x1C},
//  {CC112X_PREAMBLE_CFG1,       0x14},
//  {CC112X_PREAMBLE_CFG0,       0x2A},
//  {CC112X_FREQ_IF_CFG,         0x40},
//  {CC112X_IQIC,                0xC6},
//  {CC112X_CHAN_BW,             0x10},
//  {CC112X_MDMCFG1,             0x46},
//  {CC112X_MDMCFG0,             0x05},
//  {CC112X_SYMBOL_RATE2,        0x43},
//  {CC112X_SYMBOL_RATE1,        0xA9},
//  {CC112X_SYMBOL_RATE0,        0x2A},
//  {CC112X_AGC_REF,             0x20},
//  {CC112X_AGC_CS_THR,          0x19},
//  {CC112X_AGC_GAIN_ADJUST,     0x00},
//  {CC112X_AGC_CFG3,            0x91},
//  {CC112X_AGC_CFG2,            0x20},
//  {CC112X_AGC_CFG1,            0xA9},
//  {CC112X_AGC_CFG0,            0xCF},
//  {CC112X_FIFO_CFG,            0x00},
//  {CC112X_DEV_ADDR,            0x00},
//  {CC112X_SETTLING_CFG,        0x0B},
//  {CC112X_FS_CFG,              0x14},
//  {CC112X_WOR_CFG1,            0x08},
//  {CC112X_WOR_CFG0,            0x21},
//  {CC112X_WOR_EVENT0_MSB,      0x00},
//  {CC112X_WOR_EVENT0_LSB,      0x00},
//  {CC112X_PKT_CFG2,            0x04},
//  {CC112X_PKT_CFG1,            0x05},
//  {CC112X_PKT_CFG0,            0x20},
//  {CC112X_RFEND_CFG1,          0x0F},
//  {CC112X_RFEND_CFG0,          0x00},
//  {CC112X_PA_CFG2,             0x7F},
//  {CC112X_PA_CFG1,             0x56},
//  {CC112X_PA_CFG0,             0x7E},
//  {CC112X_PKT_LEN,             0xFF},
//  {CC112X_IF_MIX_CFG,          0x00},
//  {CC112X_FREQOFF_CFG,         0x22},
//  {CC112X_TOC_CFG,             0x0B},
//  {CC112X_MARC_SPARE,          0x00},
//  {CC112X_ECG_CFG,             0x00},
//  {CC112X_CFM_DATA_CFG,        0x00},
//  {CC112X_EXT_CTRL,            0x01},
//  {CC112X_RCCAL_FINE,          0x00},
//  {CC112X_RCCAL_COARSE,        0x00},
//  {CC112X_RCCAL_OFFSET,        0x00},
//  {CC112X_FREQOFF1,            0x00},
//  {CC112X_FREQOFF0,            0x00},
//  {CC112X_FREQ2,               0x6D},
//  {CC112X_FREQ1,               0x71},
//  {CC112X_FREQ0,               0xFC},
//  {CC112X_IF_ADC2,             0x02},
//  {CC112X_IF_ADC1,             0xA6},
//  {CC112X_IF_ADC0,             0x04},
//  {CC112X_FS_DIG1,             0x00},
//  {CC112X_FS_DIG0,             0x5F},
//  {CC112X_FS_CAL3,             0x00},
//  {CC112X_FS_CAL2,             0x20},
//  {CC112X_FS_CAL1,             0x00},
//  {CC112X_FS_CAL0,             0x0E},
//  {CC112X_FS_CHP,              0x28},
//  {CC112X_FS_DIVTWO,           0x03},
//  {CC112X_FS_DSM1,             0x00},
//  {CC112X_FS_DSM0,             0x33},
//  {CC112X_FS_DVC1,             0xFF},
//  {CC112X_FS_DVC0,             0x17},
//  {CC112X_FS_LBI,              0x00},
//  {CC112X_FS_PFD,              0x50},
//  {CC112X_FS_PRE,              0x6E},
//  {CC112X_FS_REG_DIV_CML,      0x14},
//  {CC112X_FS_SPARE,            0xAC},
//  {CC112X_FS_VCO4,             0x14},
//  {CC112X_FS_VCO3,             0x00},
//  {CC112X_FS_VCO2,             0x00},
//  {CC112X_FS_VCO1,             0x00},
//  {CC112X_FS_VCO0,             0x81},
//  {CC112X_GBIAS6,              0x00},
//  {CC112X_GBIAS5,              0x02},
//  {CC112X_GBIAS4,              0x00},
//  {CC112X_GBIAS3,              0x00},
//  {CC112X_GBIAS2,              0x10},
//  {CC112X_GBIAS1,              0x00},
//  {CC112X_GBIAS0,              0x00},
//  {CC112X_IFAMP,               0x01},
//  {CC112X_LNA,                 0x01},
//  {CC112X_RXMIX,               0x01},
//  {CC112X_XOSC5,               0x0E},
//  {CC112X_XOSC4,               0xA0},
//  {CC112X_XOSC3,               0xC7},
//  {CC112X_XOSC2,               0x04},
//  {CC112X_XOSC1,               0x07},
//  {CC112X_XOSC0,               0x00},
//  {CC112X_ANALOG_SPARE,        0x00},
//  {CC112X_PA_CFG3,             0x00},
//  {CC112X_WOR_TIME1,           0x00},
//  {CC112X_WOR_TIME0,           0x00},
//  {CC112X_WOR_CAPTURE1,        0x00},
//  {CC112X_WOR_CAPTURE0,        0x00},
//  {CC112X_BIST,                0x00},
//  {CC112X_DCFILTOFFSET_I1,     0x00},
//  {CC112X_DCFILTOFFSET_I0,     0x00},
//  {CC112X_DCFILTOFFSET_Q1,     0x00},
//  {CC112X_DCFILTOFFSET_Q0,     0x00},
//  {CC112X_IQIE_I1,             0x00},
//  {CC112X_IQIE_I0,             0x00},
//  {CC112X_IQIE_Q1,             0x00},
//  {CC112X_IQIE_Q0,             0x00},
//  {CC112X_RSSI1,               0x80},
//  {CC112X_RSSI0,               0x00},
//  {CC112X_MARCSTATE,           0x41},
//  {CC112X_LQI_VAL,             0x00},
//  {CC112X_PQT_SYNC_ERR,        0xFF},
//  {CC112X_DEM_STATUS,          0x00},
//  {CC112X_FREQOFF_EST1,        0x00},
//  {CC112X_FREQOFF_EST0,        0x00},
//  {CC112X_AGC_GAIN3,           0x00},
//  {CC112X_AGC_GAIN2,           0xD1},
//  {CC112X_AGC_GAIN1,           0x00},
//  {CC112X_AGC_GAIN0,           0x3F},
//  {CC112X_CFM_RX_DATA_OUT,     0x00},
//  {CC112X_CFM_TX_DATA_IN,      0x00},
//  {CC112X_ASK_SOFT_RX_DATA,    0x30},
//  {CC112X_RNDGEN,              0x7F},
//  {CC112X_MAGN2,               0x00},
//  {CC112X_MAGN1,               0x00},
//  {CC112X_MAGN0,               0x00},
//  {CC112X_ANG1,                0x00},
//  {CC112X_ANG0,                0x00},
//  {CC112X_CHFILT_I2,           0x08},
//  {CC112X_CHFILT_I1,           0x00},
//  {CC112X_CHFILT_I0,           0x00},
//  {CC112X_CHFILT_Q2,           0x00},
//  {CC112X_CHFILT_Q1,           0x00},
//  {CC112X_CHFILT_Q0,           0x00},
//  {CC112X_GPIO_STATUS,         0x00},
//  {CC112X_FSCAL_CTRL,          0x01},
//  {CC112X_PHASE_ADJUST,        0x00},
//  {CC112X_PARTNUMBER,          0x58},
//  {CC112X_PARTVERSION,         0x23},
//  {CC112X_SERIAL_STATUS,       0x00},
//  {CC112X_MODEM_STATUS1,       0x10},
//  {CC112X_MODEM_STATUS0,       0x00},
//  {CC112X_MARC_STATUS1,        0x00},
//  {CC112X_MARC_STATUS0,        0x00},
//  {CC112X_PA_IFAMP_TEST,       0x00},
//  {CC112X_FSRF_TEST,           0x00},
//  {CC112X_PRE_TEST,            0x00},
//  {CC112X_PRE_OVR,             0x00},
//  {CC112X_ADC_TEST,            0x00},
//  {CC112X_DVC_TEST,            0x0B},
//  {CC112X_ATEST,               0x40},
//  {CC112X_ATEST_LVDS,          0x00},
//  {CC112X_ATEST_MODE,          0x00},
//  {CC112X_XOSC_TEST1,          0x3C},
//  {CC112X_XOSC_TEST0,          0x00},
//  {CC112X_RXFIRST,             0x00},
//  {CC112X_TXFIRST,             0x00},
//  {CC112X_RXLAST,              0x00},
//  {CC112X_TXLAST,              0x00},
//  {CC112X_NUM_TXBYTES,         0x00},
//  {CC112X_NUM_RXBYTES,         0x00},
//  {CC112X_FIFO_NUM_TXBYTES,    0x0F},
//  {CC112X_FIFO_NUM_RXBYTES,    0x00},
//};




#endif /* INTERFACE_RADIO_H_ */
