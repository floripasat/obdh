// *************************************************************************************
//
// Filename:  hal_SPI.h: 
// Declarations for Communication with the MMC (see mmc.c) in unprotected SPI mode.
//
// Version 1.1
//    added ul declaration in macros mmcWriteSector and mmcReadSector
// *************************************************************************************

#ifndef _SPILIB_H
#define _SPILIB_H

#include "hal_hardware_board.h"

//----------------------------------------------------------------------------
//  These constants are used to identify the chosen SPI and UART
//  interfaces.
//----------------------------------------------------------------------------
#define SER_INTF_NULL    0
#define SER_INTF_USART0  1
#define SER_INTF_USART1  2
#define SER_INTF_USCIA0  3
#define SER_INTF_USCIA1  4
#define SER_INTF_USCIB0  5
#define SER_INTF_USCIB1  6
#define SER_INTF_USI     7
#define SER_INTF_BITBANG 8


#if SPI_SER_INTF == SER_INTF_USART0
 #define halSPIRXBUF  U0RXBUF
 #define halSPI_SEND(x) U0TXBUF=x
 #define halSPITXREADY  (IFG1&UTXIFG0)         /* Wait for TX to be ready */
 #define halSPITXDONE  (U0TCTL&TXEPT)          /* Wait for TX to finish */
 #define halSPIRXREADY (IFG1&URXIFG0)          /* Wait for TX to be ready */
 #define halSPIRXFG_CLR IFG1 &= ~URXIFG0
 #define halSPI_PxIN  SPI_USART0_PxIN
 #define halSPI_SOMI  SPI_USART0_SOMI

 #elif SPI_SER_INTF == SER_INTF_USART1
 #define halSPIRXBUF  U1RXBUF
 #define halSPI_SEND(x) U1TXBUF=x
 #define halSPITXREADY  (IFG2&UTXIFG1)         /* Wait for TX to be ready */
 #define halSPITXDONE  (U1TCTL&TXEPT)          /* Wait for TX to finish */
 #define halSPIRXREADY (IFG2&URXIFG1)          /* Wait for TX to be ready */
 #define halSPIRXFG_CLR IFG2 &= ~URXIFG1
 #define halSPI_PxIN  SPI_USART1_PxIN
 #define halSPI_SOMI  SPI_USART1_SOMI

 #elif SPI_SER_INTF == SER_INTF_USCIA0
 #define halSPIRXBUF  U0RXBUF
 #define halSPI_SEND(x) U0TXBUF=x
 #define halSPITXREADY  (IFG1&UTXIFG0)         /* Wait for TX to be ready */
 #define halSPITXDONE  (UCA0STAT&UCBUSY)       /* Wait for TX to finish */
 #define halSPIRXREADY (IFG1&URXIFG0)          /* Wait for TX to be ready */
 #define halSPIRXFG_CLR IFG1 &= ~URXIFG0
 #define halSPI_PxIN  SPI_USART0_PxIN
 #define halSPI_SOMI  SPI_USART0_SOMI

 #elif SPI_SER_INTF == SER_INTF_USCIA1
 #define halSPIRXBUF  UCA1RXBUF
 #define halSPI_SEND(x) UCA1TXBUF=x
 #define halSPITXREADY  (UCA1IFG&UCTXIFG)     /* Wait for TX to be ready */
 #define halSPITXDONE  (UCA1IFG&UCTXIFG)       /* Wait for TX to finish */
 #define halSPIRXREADY (UCA1IFG&UCRXIFG)      /* Wait for TX to be ready */
// #define halSPIRXFG_CLR IFG1 &= ~URXIFG0
// #define halSPI_PxIN  SPI_USART0_PxIN
// #define halSPI_SOMI  SPI_USART0_SOMI

 #elif SPI_SER_INTF == SER_INTF_USCIB0
 #define halSPIRXBUF  UCB0RXBUF
 #define halSPI_SEND(x) UCB0TXBUF=x
 #define halSPITXREADY  (UC0IFG&UCB0TXIFG)     /* Wait for TX to be ready */
 #define halSPITXDONE  (UCB0STAT&UCBUSY)       /* Wait for TX to finish */
 #define halSPIRXREADY (UC0IFG&UCB0RXIFG)      /* Wait for TX to be ready */
 #define halSPIRXFG_CLR UC0IFG &= ~UCB0RXIFG
 #define halSPI_PxIN  SPI_USART0_PxIN
 #define halSPI_SOMI  SPI_USART0_SOMI

 #elif SPI_SER_INTF == SER_INTF_USCIB1
 #define halSPIRXBUF  UCB1RXBUF
 #define halSPI_SEND(x) UCB1TXBUF=x
 #define halSPITXREADY  (UCB1IFG&UCTXIFG)     /* Wait for TX to be ready */
 #define halSPITXDONE  (UCB1IFG&UCTXIFG)       /* Wait for TX to finish */
 #define halSPIRXREADY (UCB1IFG&UCRXIFG)      /* Wait for TX to be ready */
 #define halSPIRXFG_CLR IFG1 &= ~URXIFG0
 #define halSPI_PxIN  SPI_USART0_PxIN
 #define halSPI_SOMI  SPI_USART0_SOMI

 #elif SPI_SER_INTF == SER_INTF_USI
 #define halSPIRXBUF  USISRL
 #define halSPI_SEND(x) USISRL = x; USICNT = 8
 #define halSPITXREADY (USICTL1&USIIFG)
 #define halSPITXDONE  (1)
 #define halSPIRXREADY  1
 #define halSPIRXFG_CLR
 #define halSPI_PxIN  SPI_BITBANG_PxIN
 #define halSPI_SOMI  SPI_BITBANG_SOMI

 #elif SPI_SER_INTF == SER_INTF_BITBANG
 #define halSPIRXBUF  spi_bitbang_in_data
 #define halSPI_SEND(x) spi_bitbang_inout(x)
 #define halSPITXREADY  1
 #define halSPITXDONE   1
 #define halSPIRXREADY  1
 #define halSPIRXFG_CLR
 #define halSPI_PxIN  SPI_BITBANG_PxIN
 #define halSPI_SOMI  SPI_BITBANG_SOMI
#endif


// Varialbes


// Function Prototypes
void halSPISetup (void);
unsigned char spiSendByte(const unsigned char data);
unsigned char spiReadFrame(unsigned char* pBuffer, unsigned int size);
unsigned char spiSendFrame(unsigned char* pBuffer, unsigned int size);


#endif /* _SPILIB_H */
