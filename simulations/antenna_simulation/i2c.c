//*****************************************************************************
//	Hardware setup
//                                /|\  /|\
//                MSP430F5529     10k  10k     MSP430F5529
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//           -|XIN  P3.0/UCB0SDA|<-|----+->|P3.0/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//           -|XOUT             |  |       |             XOUT|-
//            |     P3.1/UCB0SCL|<-+------>|P3.1/UCB0SCL     |
//            |                 |          |                 |
//******************************************************************************

#include "i2c.h"
#include "antenna.h"

uint8_t ant_command;

void SetupI2C(void) {

    P3REN |= BIT0 | BIT1;                               // Enable resistor on P3.0 and P3.1
    P3OUT  = BIT0 | BIT1;                               // Set resistor to pull-up, P3.0 and P3.1 high
    P3SEL |= BIT0 | BIT1;                               // Assign I2C pins to USCI_B0
    UCB0CTL1 |= UCSWRST;                                // Enable SW reset
    UCB0CTL0 = UCMODE_3 + UCSYNC;                       // I2C Slave, synchronous mode
    UCB0I2COA = SLAVE_ADDRESS_A;
    UCB0CTL1 &= ~UCSWRST;                               // Clear SW reset, resume operation
    UCB0IE |= UCSTPIE | UCSTTIE | UCRXIE | UCTXIE;      // Enable STT, STP, TX & RX interrupt
    __bis_SR_register(GIE);
}

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
    switch(__even_in_range(UCB0IV,12))
    {
    case  0: break;                                     // Vector  0: No interrupts
    case  2: break;                                     // Vector  2: ALIFG
    case  4: break;                                     // Vector  4: NACKIFG

    case  6:                                            // Vector  6: STTIFG
        UCB0IFG &= ~UCSTTIFG;
        break;

    case  8:                                            // Vector  8: STPIFG
        UCB0IFG &= ~UCSTPIFG;
        break;

    case 10:                                              // Vector 10: RXIFG
        switch_command(UCB0RXBUF);
        break;

    case 12:                                  					// Vector 12: TXIFG

        //envia uma resposta dependendo do comando enviado

        break;

    default: break;
    }
}
