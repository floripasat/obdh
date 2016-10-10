/******************************************************************************************/
/*
 * clocks.c
 *
 *  Created on: 21 de jun de 2016
 *      Author: mfrata
 *
 */
/******************************************************************************************/

#include <driver/clocks.h>

void MAIN_clocks_setup(void) {

      P7SEL |= BIT2|BIT3;                       // Port select XT2

      UCSCTL6 &= ~XT2OFF;                       // Enable XT2
      UCSCTL3 |= SELREF_2;                      // FLLref = REFO
                                                // Since LFXT1 is not used,
                                                // sourcing FLL with LFXT1 can cause
                                                // XT1OFFG flag to set
      UCSCTL4 |= SELA_2;                        // ACLK=REFO,SMCLK=DCO,MCLK=DCO

      // Loop until XT1,XT2 & DCO stabilizes - in this case loop until XT2 settles
//      do
//      {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | XT1HFOFFG | DCOFFG);
                                                // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
//      }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

      UCSCTL6 &= ~XT2DRIVE0;                    // Decrease XT2 Drive according to
                                                // expected frequency
      UCSCTL4 |= SELS_5 | SELM_5;               // SMCLK=MCLK=XT2
      UCSCTL5 |= DIVS1;                         //SMCLK source divider = 4

// Uncomment this to see the clocks output to the pins below

    P1DIR |= BIT0;                            // ACLK set out to pins
    P1SEL |= BIT0;
//
//    P3DIR |= BIT4;                            // SMCLK set out to pins
//    P3SEL |= BIT4;
}
/******************************************************************************************/
