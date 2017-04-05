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
    setup_dco();
    setup_xt1_xt2();
    setup_clks();
    test_flag = test_fault_flags();

    return test_flag;
}

void setup_dco(){

    __bis_SR_register(SCG0);                  // Disable the FLL control loop
    UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_6;                      // Select DCO range 32MHz operation
    UCSCTL2 = FLLD_0 + 975;                   // Set DCO Multiplier for 32MHz
                                              // (N + 1) * FLLRef = Fdco
                                              // (975 + 1) * 32768 = 32MHz
                                              // Set FLL Div = fDCOCLK/(1+0)
    UCSCTL3 = SELREF__REFOCLK | FLLREFDIV_0;
    __bic_SR_register(SCG0);                  // Enable the FLL control loop
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
    UCSCTL4 |= SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK;        // SMCLK = MCLK = XT2 , ACLK = XT1
}

uint8_t test_fault_flags(void){
    uint16_t attempts = 0;
    uint8_t result = 0xFF;

    do {
        _delay_cycles(500); //1ms delay (DCO at ~500KHz)
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | XT1HFOFFG | DCOFFG);  // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
        attempts++;
    } while ((SFRIFG1 & OFIFG) && attempts < XT_MAX_STARTUP_TIME ); //try for 30 seconds. The datasheet says 1s of typical Startup time.

    if( attempts < XT_MAX_STARTUP_TIME){
        result = TEST_SUCESS;
    }
    else {
        result = TEST_FAIL;
    }
    return result;
}
