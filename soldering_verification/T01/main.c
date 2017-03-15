#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    volatile unsigned int i;

    P5DIR |= BIT3;

    while(1)
    {
        P5OUT ^= BIT3;                          //toggle LED_System
        __delay_cycles(3200000);                   // Delay
    }
	return 0;
}
