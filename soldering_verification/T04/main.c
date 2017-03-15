#include <msp430.h> 

/*
 * main.c
 */

void setup_xt1();
void setup_xt2();
void setup_xt1_xt2();
void setup_clks();
void test_fault_flags();
void blink_led();

volatile unsigned int i;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

//    setup_xt1();
//    setup_xt2();
    setup_xt1_xt2();
    test_fault_flags();
    setup_clks();

    blink_led();

	return 0;
}


void setup_xt1()
{
    while(BAKCTL & LOCKBAK)                   // Unlock XT1 pins for operation
        BAKCTL &= ~(LOCKBAK);

    UCSCTL6 &= ~(XT1OFF);            // Enable XT1
    UCSCTL6 |= XCAP_3;                        // Internal load cap
//    UCSCTL6 |= XT1DRIVE_3;
}

void setup_xt2()
{
    P7SEL |= BIT2 + BIT3; //XT2

    UCSCTL6 |= XT1OFF;
    UCSCTL6 &= ~XT2OFF;            // Enable XT2
    UCSCTL6 |= XCAP_3;                        // Internal load cap
    UCSCTL3 |= SELREF_2;                      // FLLref = REFO
                                                // Since LFXT1 is not used,
                                                // sourcing FLL with LFXT1 can cause
                                                // XT1OFFG flag to set

//    UCSCTL6 &= ~XT2DRIVE_0;                  //4MHz crystal
}

void setup_xt1_xt2()
{
    while(BAKCTL & LOCKBAK)                   // Unlock XT1 pins for operation
        BAKCTL &= ~(LOCKBAK);

    P7SEL |= BIT2 + BIT3; //XT2
    UCSCTL6 &= ~(XT1OFF | XT2OFF);            // Enable XT1 & XT2
    UCSCTL6 |= XCAP_3;                        // Internal load cap
}

void setup_clks()
{
    P1DIR |= BIT0;    // ACLK set out to pin
    P1SEL |= BIT0;

//    P3DIR |= BIT4;    // SMCLK set out to pin
//    P3SEL |= BIT4;

    UCSCTL5 |= DIVM_1 + DIVS_1;
    UCSCTL4 |= SELA_0 + SELS_5 + SELM_5;        // SMCLK = MCLK = XT2 , ACLK = XT1
}

void test_fault_flags()
{
    do {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);  // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
    } while (SFRIFG1 & OFIFG);
}

void blink_led()
{
    P5DIR |= BIT3;                 // P5.3 set as led system output

    while(1) {
        P5OUT ^= BIT3;                          // Toggle LED_SYSTEM
        _delay_cycles(8000000);                   // Delay
    }
}
