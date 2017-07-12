#include <msp430.h>
#include <stdint.h>

#include "i2c.h"
#include "antenna.h"

int main(void) {

    WDTCTL = WDTPW + WDTHOLD;

    P1DIR |= BIT0;                // LED P1.0 ARMING
    P4DIR |= BIT7;                // LED P4.7 DEPLOY

    P1OUT &= ~BIT0;
    P4OUT &= ~BIT7;

    P1REN |= BIT1;                // Enable resistor
    P1OUT |= BIT1;                 // Set as pull-up

    SetupI2C();

    while(1);
}
