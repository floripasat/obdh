#include <msp430.h>
#include <stdint.h>

#include "i2c.h"
#include "antenna.h"

int main(void) {

  uint8_t command;

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
        arming_ant();
        break;

      case DISARMING:
        disarming_ant();
        break;

      case DEPLOY_ANT_1:
        deploy_ant(1);
        break;

      case DEPLOY_ANT_2:
        deploy_ant(2);
        break;

      case DEPLOY_ANT_3:
        deploy_ant(3);
        break;

      case DEPLOY_ANT_4:
        deploy_ant(4);
        break;

      case DEPLOY_SEQUENCIAL:
        deploy_sequencial_ant();
        break;

      default:
        break;
    }

    command = 0;
  }
}
