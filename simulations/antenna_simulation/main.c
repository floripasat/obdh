#include <msp430.h>
#include <stdint.h>

#include "i2c.h"
#include "antenna.h"

#define CLOSE               1
#define OPEN                0
#define switch_control()    (P1IN & BIT1)

int main(void) {

  uint8_t command;
  uint8_t armed;
  uint8_t STATUS_ANT_1 = CLOSE, STATUS_ANT_2 = CLOSE, STATUS_ANT_3 = CLOSE, STATUS_ANT_4 = CLOSE;

  WDTCTL = WDTPW + WDTHOLD;

  P1DIR |= BIT0;                 // LED P1.0 ARMING
  P4DIR |= BIT7;                 // LED P4.7 DEPLOY

  P1REN |= BIT1;                 // Enable resistor
  P1OUT |= BIT1;                 // Set as pull-up

  SetupI2C();

  while(1) {     

    get_command (&command);

    switch (command) {
      case ARMING:

        if (switch_control() == CLOSE) {
          armed = 1;
          P1OUT = BIT0;
        }

        break;

      case DISARMING:

        if (armed == 1) {
          armed = 0;
          P1OUT = ~BIT0;
        }

        break;

      case DEPLOY_ANT_1:

        while (switch_control() == CLOSE) {
          P4OUT ^= BIT7;
          __delay_cycles(1000);
        }

        STATUS_ANT_2 = OPEN;
        P4OUT = BIT7;
        break;

      case DEPLOY_ANT_2:

        while (switch_control() == CLOSE) {
          P4OUT ^= BIT7;
          __delay_cycles(1000);
        }

        STATUS_ANT_2 = OPEN;
        P4OUT = BIT7;
        break;

      case DEPLOY_ANT_3:

        while (switch_control() == CLOSE) {
          P4OUT ^= BIT7;
          __delay_cycles(1000);
        }

        STATUS_ANT_3 = OPEN;
        P4OUT = BIT7;
        break;

      case DEPLOY_ANT_4:

        while (switch_control() == CLOSE) {
          P4OUT ^= BIT7;
          __delay_cycles(1000);
        }

        STATUS_ANT_4 = OPEN;
        P4OUT = BIT7;
        break;

      case DEPLOY_SEQUENCIAL:

        if (STATUS_ANT_1 || STATUS_ANT_2 || STATUS_ANT_3 || STATUS_ANT_4) {

          while (STATUS_ANT_1) {
            P4OUT ^= BIT7;
            __delay_cycles(1000);
          }

          STATUS_ANT_1 = OPEN;
          P4OUT = BIT7;

          while (STATUS_ANT_2) {
            P4OUT ^= BIT7;
            __delay_cycles(1000);
          }

          STATUS_ANT_2 = OPEN;
          P4OUT = BIT7;

          while (STATUS_ANT_3) {
            P4OUT ^= BIT7;
            __delay_cycles(1000);
          }

          STATUS_ANT_3 = OPEN;
          P4OUT = BIT7;

          while (STATUS_ANT_4) {
            P4OUT ^= BIT7;
            __delay_cycles(1000);
          }

          STATUS_ANT_4 = OPEN;
          P4OUT = BIT7;
        }

        break;

      default:
        break;
    }

    command = 0;
  }
}
