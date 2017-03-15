/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//   MSP430F665x Demo - eUSCI_A0, SPI 3-Wire Master Incremented Data
//
//   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
//   data is sent by the master starting at 0x01. Received data is expected to
//   be same as the previous transmission.  eUSCI RX ISR is used to handle
//   communication with the CPU, normally in LPM0. If high, P1.0 indicates
//   valid data reception.  Because all execution after LPM0 is in ISRs,
//   initialization waits for DCO to stabilize against ACLK.
//   
//   ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~ 1048kHz.  BRCLK = SMCLK/2
//
//   Use with SPI Slave Data Echo code example. The master code example uses
//   a GPIO P1.1 to reset the SPI slave state machine.
//
//   Note that UCB0CLK, UCB0SIMO and UCB0SOMI need to be assigned pins via the 
//   PMAP controller.
//
//                     MSP430F665x
//                   -----------------
//               /|\|                 |
//                | |                 |
//                --|RST          P1.0|-> LED
//                  |                 |
//                  |             P2.2|-> Data Out (UCA1SIMO)
//                  |                 |
//                  |             P2.1|<- Data In (UCA1SOMI)
//                  |                 |
//Slave SPI reset <-|P1.1         P2.0|-> Serial Clock Out (UCA1CLK)
//
//
//   P. Thanigai
//   Texas Instruments Inc.
//   October 2012
//   Built with IAR Embedded Workbench Version: 5.40 & CCS V5.2
//******************************************************************************
#include <msp430.h>



#define MEM0_ENABLE()     (P7OUT &= ~BIT5) // MEM0_CE enable
#define MEM1_ENABLE()     (P7OUT &= ~BIT6) // MEM0_CE enable
#define MEM2_ENABLE()     (P7OUT &= ~BIT7) // MEM0_CE enable
#define WAIT_SPI_SENT()         while (!(UCA1IFG & UCTXIFG))
#define WAIT_SPI_RECEIVED()         while (!(UCA1IFG & UCRXIFG))


unsigned char MST_Data,SLV_Data;
unsigned char temp;

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW|WDTHOLD;                   // Stop watchdog timer
  
  P8SEL |= BIT1|BIT2|BIT3;                  // Assign P2.0 to UCB0CLK and...
  P8DIR |=  BIT1|BIT2|BIT3;                 // P2.1 UCB0SOMI P2.2 UCB0SIMO

  // MEM_WP | MEM0_CE
  P7DIR |= BIT4 | BIT5 | BIT6 | BIT7;
  P7OUT &= ~(BIT5 | BIT6 | BIT7); // MEMx_CE disable
//  P7OUT &= ~BIT5; // MEM0_CE enable
  P7OUT |= BIT4; // MEM_WP disable

  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL0 |= UCMST|UCSYNC|UCCKPL|UCMSB;    // 3-pin, 8-bit SPI master
                                            // Clock polarity high, MSB
  UCA1CTL1 |= UCSSEL_2;                     // SMCLK
  UCA1BR0 = 0x02;                           // /2
  UCA1BR1 = 0;                              //
  UCA1MCTL = 0;                             // No modulation
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
//  UCA1IE = UCRXIE ;                         // Enable USCI_A0 RX,TX interrupt

  MEM0_ENABLE();                          // chip select for slave
//  MEM1_ENABLE();
//  MEM2_ENABLE();

  __delay_cycles(100);                      // Wait for slave to initialize
 
  MST_Data = 0x9F;                          // Read CHIP ID

  while (!(UCA1IFG & UCTXIFG));               // USCI_A0 TX buffer ready?
  UCA1TXBUF = MST_Data;                     // Transmit first character
  WAIT_SPI_SENT();
//  UCA1TXBUF = 0; //dummy
//  WAIT_SPI_SENT();
//  UCA1TXBUF = 0; //dummy
//  WAIT_SPI_SENT();
//  UCA1TXBUF = 0; //dummy
//  WAIT_SPI_SENT();

  UCA1TXBUF = 0; //dummy
  WAIT_SPI_SENT();
  WAIT_SPI_RECEIVED();
  SLV_Data = UCA1RXBUF;

  UCA1TXBUF = 0; //dummy
  WAIT_SPI_SENT();
  WAIT_SPI_RECEIVED();
  SLV_Data = UCA1RXBUF;

  UCA1TXBUF = 0; //dummy
  WAIT_SPI_SENT();
  WAIT_SPI_RECEIVED();
  SLV_Data = UCA1RXBUF;
  while(1);
}

