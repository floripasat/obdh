//----------------------------------------------------------------------------
//  This file contains functions that allow the MSP430 device to access the
//  SPI interface.  There are multiple instances of each function; 
//  the one to be compiled is selected by the system variable
//  SPI_SER_INTF, defined in "hal_hardware_board.h".
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  void halSPISetup(void)
//
//  DESCRIPTION:
//  Configures the assigned interface to function as a SPI port and
//  initializes it.
//----------------------------------------------------------------------------
//  void halSPIWriteReg(char addr, char value)
//
//  DESCRIPTION:
//  Writes "value" to a single configuration register at address "addr".
//----------------------------------------------------------------------------
//  void halSPIWriteBurstReg(char addr, char *buffer, char count)
//
//  DESCRIPTION:
//  Writes values to multiple configuration registers, the first register being
//  at address "addr".  First data byte is at "buffer", and both addr and
//  buffer are incremented sequentially (within the CCxxxx and MSP430,
//  respectively) until "count" writes have been performed.
//----------------------------------------------------------------------------
//  char halSPIReadReg(char addr)
//
//  DESCRIPTION:
//  Reads a single configuration register at address "addr" and returns the
//  value read.
//----------------------------------------------------------------------------
//  void halSPIReadBurstReg(char addr, char *buffer, char count)
//
//  DESCRIPTION:
//  Reads multiple configuration registers, the first register being at address
//  "addr".  Values read are deposited sequentially starting at address
//  "buffer", until "count" registers have been read.
//----------------------------------------------------------------------------
//  char halSPIReadStatus(char addr)
//
//  DESCRIPTION:
//  Special read function for reading status registers.  Reads status register
//  at register "addr" and returns the value read.
//----------------------------------------------------------------------------
//  void halSPIStrobe(char strobe)
//
//  DESCRIPTION:
//  Special write function for writing to command strobe registers.  Writes
//  to the strobe at address "addr".
//----------------------------------------------------------------------------

/* ***********************************************************
* THIS PROGRAM IS PROVIDED "AS IS". TI MAKES NO WARRANTIES OR
* REPRESENTATIONS, EITHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS 
* FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR 
* COMPLETENESS OF RESPONSES, RESULTS AND LACK OF NEGLIGENCE. 
* TI DISCLAIMS ANY WARRANTY OF TITLE, QUIET ENJOYMENT, QUIET 
* POSSESSION, AND NON-INFRINGEMENT OF ANY THIRD PARTY 
* INTELLECTUAL PROPERTY RIGHTS WITH REGARD TO THE PROGRAM OR 
* YOUR USE OF THE PROGRAM.
*
* IN NO EVENT SHALL TI BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
* CONSEQUENTIAL OR INDIRECT DAMAGES, HOWEVER CAUSED, ON ANY 
* THEORY OF LIABILITY AND WHETHER OR NOT TI HAS BEEN ADVISED 
* OF THE POSSIBILITY OF SUCH DAMAGES, ARISING IN ANY WAY OUT 
* OF THIS AGREEMENT, THE PROGRAM, OR YOUR USE OF THE PROGRAM. 
* EXCLUDED DAMAGES INCLUDE, BUT ARE NOT LIMITED TO, COST OF 
* REMOVAL OR REINSTALLATION, COMPUTER TIME, LABOR COSTS, LOSS 
* OF GOODWILL, LOSS OF PROFITS, LOSS OF SAVINGS, OR LOSS OF 
* USE OR INTERRUPTION OF BUSINESS. IN NO EVENT WILL TI'S 
* AGGREGATE LIABILITY UNDER THIS AGREEMENT OR ARISING OUT OF 
* YOUR USE OF THE PROGRAM EXCEED FIVE HUNDRED DOLLARS 
* (U.S.$500).
*
* Unless otherwise stated, the Program written and copyrighted 
* by Texas Instruments is distributed as "freeware".  You may, 
* only under TI's copyright in the Program, use and modify the 
* Program without any charge or restriction.  You may 
* distribute to third parties, provided that you transfer a 
* copy of this license to the third party and the third party 
* agrees to these terms by its first use of the Program. You 
* must reproduce the copyright notice and any other legend of 
* ownership on each copy or partial copy, of the Program.
*
* You acknowledge and agree that the Program contains 
* copyrighted material, trade secrets and other TI proprietary 
* information and is protected by copyright laws, 
* international copyright treaties, and trade secret laws, as 
* well as other intellectual property laws.  To protect TI's 
* rights in the Program, you agree not to decompile, reverse 
* engineer, disassemble or otherwise translate any object code 
* versions of the Program to a human-readable form.  You agree 
* that in no event will you alter, remove or destroy any 
* copyright notice included in the Program.  TI reserves all 
* rights not specifically granted under this license. Except 
* as specifically provided herein, nothing in this agreement 
* shall be construed as conferring by implication, estoppel, 
* or otherwise, upon you, any license or other right under any 
* TI patents, copyrights or trade secrets.
*
* You may not use the Program in non-TI devices.
* ********************************************************* */


#ifndef _SPILIB_C
#define _SPILIB_C
//
//---------------------------------------------------------------
#include "hal_SPI.h"
#include "hal_hardware_board.h"

//#define withDMA

#ifndef DUMMY_CHAR
#define DUMMY_CHAR 0xFF
#endif

// SPI port functions
#if SPI_SER_INTF == SER_INTF_USART0

void halSPISetup(void)
{
  UCTL0 = CHAR + SYNC + MM + SWRST;         // 8-bit SPI Master **SWRST**
  UTCTL0 = CKPL + SSEL1 + SSEL0 + STC;      // SMCLK, 3-pin mode
  UBR00 = 0x02;                             // UCLK/2
  UBR10 = 0x00;                             // 0
  UMCTL0 = 0x00;                            // No modulation
  ME1 |= USPIE0;                            // Enable USART0 SPI mode
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
}

#elif SPI_SER_INTF == SER_INTF_USART1

void halSPISetup(void)
{
  UCTL1 = CHAR + SYNC + MM + SWRST;         // 8-bit SPI Master **SWRST**
  UTCTL1 = CKPL + SSEL1 + SSEL0 + STC;      // SMCLK, 3-pin mode
  UBR01 = 0x02;                             // UCLK/2
  UBR11 = 0x00;                             // 0
  UMCTL1 = 0x00;                            // No modulation
  ME2 |= USPIE1;                            // Enable USART1 SPI mode
  UCTL1 &= ~SWRST;                          // Initialize USART state machine
}

#elif SPI_SER_INTF == SER_INTF_USCIA0

void halSPISetup(void)
{
  UCA0CTL0 = UCMST+UCCKPL+UCMSB+UCSYNC;     // 3-pin, 8-bit SPI master
  UCA0CTL1 = UCSSEL_2 + UCSWRST;            // SMCLK
  UCA0BR0 |= 0x02;                          // UCLK/2
  UCA0BR1 = 0;
  UCA0MCTL = 0;
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

#elif SPI_SER_INTF == SER_INTF_USCIA1

void halSPISetup(void)
{
  UCA1CTL0 = UCMST+UCCKPL+UCMSB+UCSYNC;     // 3-pin, 8-bit SPI master
  UCA1CTL1 = UCSSEL_2 + UCSWRST;            // SMCLK
  UCA1BR0 |= 0x02;                          // UCLK/2
  UCA1BR1 = 0;
  UCA1MCTL = 0;
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

#elif SPI_SER_INTF == SER_INTF_USCIB0

void halSPISetup(void)
{
  UCB0CTL0 = UCMST+UCCKPL+UCMSB+UCSYNC;     // 3-pin, 8-bit SPI master
  UCB0CTL1 = UCSSEL_2+UCSWRST;              // SMCLK
  UCB0BR0 |= 0x02;                          // UCLK/2
  UCB0BR1 = 0;
  //UCB0MCTL = 0;
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

#elif SPI_SER_INTF == SER_INTF_USCIB1

void halSPISetup(void)
{
  UCB1CTL0 = UCMST+UCCKPL+UCMSB+UCSYNC;     // 3-pin, 8-bit SPI master
  UCB1CTL1 = UCSSEL_2+UCSWRST;              // SMCLK
  UCB1BR0 |= 0x02;                          // UCLK/2
  UCB1BR1 = 0;
//  UCB1MCTL = 0;
  UCB1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

#elif SPI_SER_INTF == SER_INTF_USI

void halSPISetup(void)
{
  USICTL0 = USIPE7+USIPE6+USIPE5+USIMST+USIOE+UCSWRST; // Port, SPI master
  USICKCTL = USISSEL_2 + USICKPL;           // SCLK = SMCLK
  USICTL0 &= ~USISWRST;                     // USI released for operation

  USISRL = 0x00;                            // Ensure SDO low instead of high,
  USICNT = 1;                               // to avoid conflict with CCxxxx
}

#elif SPI_SER_INTF == SER_INTF_BITBANG

void spi_bitbang_out(unsigned char);
unsigned char spi_bitbang_in();
unsigned char spi_bitbang_in_data;

void halSPISetup(void)
{
}

// Output eight-bit value using selected bit-bang pins
void spi_bitbang_out(unsigned char value)
{
  char x;

  for(x=8;x>0;x--)
  {

    if(value & 0x80)                        // If bit is high...
      MMC_PxOUT |= MMC_SIMO;// Set SIMO high...
    else
      MMC_PxOUT &= ~MMC_SIMO;//Set SIMO low...
    value = value << 1;                     // Rotate bits

    MMC_PxOUT &= ~MMC_UCLK; // Set clock low
    MMC_PxOUT |= MMC_UCLK;  // Set clock high
  }
}

// Input eight-bit value using selected bit-bang pins
unsigned char spi_bitbang_in()
{
  char x=0;
  int y;

  for(y=8;y>0;y--)
  {
    MMC_PxOUT &= ~MMC_UCLK; // Set clock low
    MMC_PxOUT |= MMC_UCLK;  // Set clock high

    x = x << 1;                             // Rotate bits
    if(MMC_PxIN & MMC_SOMI)                 // If bit is high...
      x |= 0x01;                            // input bit high
  }
  spi_bitbang_in_data = x;
  return(x);
}
// Input eight-bit value using selected bit-bang pins
unsigned char spi_bitbang_inout(unsigned char value)
{
  char x=0;
  int y;

  for(y=8;y>0;y--)
  {
    if(value & 0x80)                        // If bit is high...
      MMC_PxOUT |= MMC_SIMO;// Set SIMO high...
    else
      MMC_PxOUT &= ~MMC_SIMO;//Set SIMO low...
    value = value << 1;                     // Rotate bits

    MMC_PxOUT &= ~MMC_UCLK; // Set clock low
    MMC_PxOUT |= MMC_UCLK;  // Set clock high

    x = x << 1;                             // Rotate bits
    if(MMC_PxIN & MMC_SOMI)                 // If bit is high...
      x |= 0x01;                            // input bit high
  }
  spi_bitbang_in_data = x;
  return(x);
}
#endif


//Send one byte via SPI
unsigned char spiSendByte(const unsigned char data)
{
  while (halSPITXREADY ==0);    // wait while not ready for TX
  halSPI_SEND(data);            // write
  while (halSPIRXREADY ==0);    // wait for RX buffer (full)
  return (halSPIRXBUF);
}


//Read a frame of bytes via SPI
unsigned char spiReadFrame(unsigned char* pBuffer, unsigned int size)
{
#ifndef withDMA
  unsigned long i = 0;
  // clock the actual data transfer and receive the bytes; spi_read automatically finds the Data Block
  for (i = 0; i < size; i++){
    while (halSPITXREADY == 0);   // wait while not ready for TX
    halSPI_SEND(DUMMY_CHAR);     // dummy write
    while (halSPIRXREADY == 0);   // wait for RX buffer (full)
    pBuffer[i] = halSPIRXBUF;
  }
#else
        U1IFG &= ~(URXIFG1 + URXIFG1);      /* clear flags */
        /* Get the block */
        /* DMA trigger is UART1 receive for both DMA0 and DMA1 */
        DMACTL0 &= ~(DMA0TSEL_15 | DMA1TSEL_15);
        DMACTL0 |= (DMA0TSEL_9 | DMA1TSEL_9);
        /* Source DMA address: receive register.  */
        DMA0SA = U1RXBUF_;
        /* Destination DMA address: the user data buffer. */
        DMA0DA = (unsigned short)pBuffer;
        /* The size of the block to be transferred */
        DMA0SZ = size;
        /* Configure the DMA transfer*/
        DMA0CTL =
          DMAIE   |                         /* Enable interrupt */
          DMADT_0 |                         /* Single transfer mode */
          DMASBDB |                         /* Byte mode */
          DMAEN |                           /* Enable DMA */
          DMADSTINCR1 | DMADSTINCR0;        /* Increment the destination address */

        /* We depend on the DMA priorities here.  Both triggers occur at
           the same time, since the source is identical.  DMA0 is handled
           first, and retrieves the byte.  DMA1 is triggered next, and
           sends the next byte. */
        /* Source DMA address: constant 0xFF (don't increment)*/
        DMA1SA = U1TXBUF_;
        /* Destination DMA address: the transmit buffer. */
        DMA1DA = U1TXBUF_;
        /* Increment the destination address */
        /* The size of the block to be transferred */
        DMA1SZ = count-1;
        /* Configure the DMA transfer*/
        DMA1CTL =
          DMADT_0 |                         /* Single transfer mode */
          DMASBDB |                         /* Byte mode */
          DMAEN;                            /* Enable DMA */

        /* Kick off the transfer by sending the first byte */
        halMMC_SEND(0xFF);
        _EINT(); LPM0;  // wait till done
#endif
  return(0);
}


//Send a frame of bytes via SPI
unsigned char spiSendFrame(unsigned char* pBuffer, unsigned int size)
{
#ifndef withDMA
  unsigned long i = 0;
  // clock the actual data transfer and receive the bytes; spi_read automatically finds the Data Block
  for (i = 0; i < size; i++){
    while (halSPITXREADY ==0);   // wait while not ready for TX
    halSPI_SEND(pBuffer[i]);     // write
    while (halSPIRXREADY ==0);   // wait for RX buffer (full)
    pBuffer[i] = halSPIRXBUF;
  }
#else
      /* Get the block */
      /* DMA trigger is UART send */
      DMACTL0 &= ~(DMA0TSEL_15);
      DMACTL0 |= (DMA0TSEL_9);
      /* Source DMA address: the data buffer.  */
      DMA0SA = (unsigned short)pBuffer;
      /* Destination DMA address: the UART send register. */
      DMA0DA = U1TXBUF_;
      /* The size of the block to be transferred */
      DMA0SZ = count;
      /* Configure the DMA transfer*/
      DMA0CTL =
        DMAREQ  |                           /* start transfer */
        DMADT_0 |                           /* Single transfer mode */
        DMASBDB |                           /* Byte mode */
        DMAEN |                             /* Enable DMA */
        DMASRCINCR1 | DMASRCINCR0;          /* Increment the source address */
#endif
  return(0);
}


#ifdef withDMA
#ifdef __IAR_SYSTEMS_ICC__
#if __VER__ < 200
interrupt[DACDMA_VECTOR] void DMA_isr(void)
#else
#pragma vector = DACDMA_VECTOR
__interrupt void DMA_isr(void)
#endif
#endif

#ifdef __CROSSWORKS__
void DMA_isr(void)   __interrupt[DACDMA_VECTOR]
#endif

#ifdef __TI_COMPILER_VERSION__
__interrupt void DMA_isr(void);
DMA_ISR(DMA_isr)
__interrupt void DMA_isr(void)
#endif
{
  DMA0CTL &= ~(DMAIFG);
  LPM3_EXIT;
}
#endif


//---------------------------------------------------------------------
#endif /* _SPILIB_C */
