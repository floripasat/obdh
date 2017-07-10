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

void SetupI2C(uint8_t* data) {

  uint8_t ant_command = 0;

  Data = data;
  P3REN |= 0x03;                                      // Enable resistor on P3.0 and P3.1
  P3OUT  = 0x03;                                      // Set resistor to pull-up, P3.0 and P3.1 high
  P3SEL |= 0x03;                                      // Assign I2C pins to USCI_B0
  UCB0CTL1 |= UCSWRST;                                // Enable SW reset
  UCB0CTL0 = UCMODE_3 + UCSYNC;                       // I2C Slave, synchronous mode
  UCB0I2COA = SLAVE_ADDRESS_A;                        
  UCB0CTL1 &= ~UCSWRST;                               // Clear SW reset, resume operation
  UCB0IE |= UCSTPIE | UCSTTIE | UCRXIE | UCTXIE;      // Enable STT, STP, TX & RX interrupt
  __bis_SR_register(GIE);
}

void get_command (uint8_t* command) {
  command = ant_command;
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

    //recebe o comando enviado pelo master
    //atualiza uma variavel para usar no loop do main

    ant_command = UCB0RXBUF;
    
    break;
  
  case 12:                                  					// Vector 12: TXIFG  
  	
    //envia uma resposta dependendo do comando enviado

  	break;
  
  default: break;
  }  
}
