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

uint8_t clocks_setup(void){
    uint8_t test_flag;
    //TODO: Verify if it's necessary to set the Vcore
    setup_xt1_xt2();
    test_flag = test_fault_flags();
    setup_clks();
    return test_flag;
}

void setup_xt1_xt2(void){

    while(BAKCTL & LOCKBAK)                   // Unlock XT1 pins for operation
        BAKCTL &= ~(LOCKBAK);

    XT2_N_DIR &= ~(XT2_N_PIN | XT2_P_PIN);
    XT2_N_SEL |= XT2_N_PIN | XT2_P_PIN;
    UCSCTL6 &= ~(XT1OFF | XT2OFF);            // Enable XT1 & XT2
    UCSCTL6 |= XCAP_3;                        // Internal load cap
}

void setup_clks(void){
//    P1DIR |= BIT0;    // ACLK set out to pin
//    P1SEL |= BIT0;

//    P3DIR |= BIT4;    // SMCLK set out to pin
//    P3SEL |= BIT4;

    UCSCTL5 |= DIVA__1 + DIVM__2 + DIVS__2;
    UCSCTL4 |= SELA_0 + SELS_5 + SELM_5;        // SMCLK = MCLK = XT2 , ACLK = XT1
}

uint8_t test_fault_flags(void){
    uint16_t attempts = 0;
    uint8_t result = 0xFF;

    do {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | XT1HFOFFG | DCOFFG);  // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
        attempts++;
    } while ((SFRIFG1 & OFIFG) && attempts < 500 );

    if( attempts < 500){
        result = TEST_SUCESS;
    }
    else {
        result = TEST_FAIL;
    }
    return result;
}
