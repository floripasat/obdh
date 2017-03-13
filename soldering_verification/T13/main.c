#include <msp430.h>
#include "mpu_i2c.h"
#include "mpu_9150.h"
#include "mpu_uart.h"

#define x  0
#define y  1
#define z  2

int acc[3];
static volatile char buffer[14];
unsigned i;

void main(void) {

    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    __bis_SR_register(GIE);
    P5DIR |= BIT3; //led

    setup_i2c(0x18);
    read_i2c(0x00, buffer, 2);
    __delay_cycles(500000);
    if(buffer[0] == 0xFA)
    {
        P5OUT ^= BIT3;
        __delay_cycles(500000);
        P5OUT ^= BIT3;
        __delay_cycles(500000);
    }



    setup_i2c(0x69);
    read_i2c(0x00, buffer, 2);
    __delay_cycles(500000);
    if(buffer[0] == 0x0F)
    {
        P5OUT ^= BIT3;
        __delay_cycles(500000);
        P5OUT ^= BIT3;
        __delay_cycles(500000);
    }

    while(1);

}
