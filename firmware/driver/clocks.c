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

volatile uint16_t status;

void clocks_setup(void) {
//    set_vcore_up(3);
//    status = PMM_setVCore(PMMCOREV_3);
    setup_xt1_xt2();
    test_fault_flags();
    setup_clks();
}

void set_vcore_up (unsigned int level)
{
    // Open PMM registers for write access
    PMMCTL0_H = 0xA5;
    // Make sure no flags are set for iterative sequences
    while ((PMMIFG & SVSMHDLYIFG) == 0);
    while ((PMMIFG & SVSMLDLYIFG) == 0);
    // Set SVS/SVM high side new level
    SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
    // Set SVM low side to new level
    SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
    // Wait till SVM is settled
    while ((PMMIFG & SVSMLDLYIFG) == 0);
    // Clear already set flags
    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
    // Set VCore to new level
    PMMCTL0_L = PMMCOREV0 * level;
    // Wait till new level reached
    if ((PMMIFG & SVMLIFG))
        while ((PMMIFG & SVMLVLRIFG) == 0);
    // Set SVS/SVM low side to new level
    SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
    // Lock PMM registers for write access
    PMMCTL0_H = 0x00;
}


void setup_xt1_xt2(void) {

    while(BAKCTL & LOCKBAK)                   // Unlock XT1 pins for operation
        BAKCTL &= ~(LOCKBAK);

    XT2_N_DIR &= ~(XT2_N_PIN | XT2_P_PIN);
    XT2_N_SEL |= XT2_N_PIN | XT2_P_PIN;
    UCSCTL6 &= ~(XT1OFF | XT2OFF);            // Enable XT1 & XT2
    UCSCTL6 |= XCAP_3;                        // Internal load cap
}

void setup_clks(void)
{
//    P1DIR |= BIT0;    // ACLK set out to pin
//    P1SEL |= BIT0;

//    P3DIR |= BIT4;    // SMCLK set out to pin
//    P3SEL |= BIT4;

    UCSCTL5 |= DIVA__1 + DIVM__2 + DIVS__2;
    UCSCTL4 |= SELA_0 + SELS_5 + SELM_5;        // SMCLK = MCLK = XT2 , ACLK = XT1
}

void test_fault_flags(void)
{
    do {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | XT1HFOFFG | DCOFFG);  // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
    } while (SFRIFG1 & OFIFG);
}
