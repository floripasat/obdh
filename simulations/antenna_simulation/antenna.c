#include "antenna.h"

uint8_t STATUS_ANT[4];
uint8_t armed;

void arming_ant(void) {
  if (switch_control() == CLOSE) {
    armed = 1;
    P1OUT = BIT0;
  }
}

void disarming_ant(void) {
  if (armed == 1) {
    armed = 0;
    P1OUT = ~BIT0;
  }
}

void deploy_ant(uint8_t antenna) {
  while (switch_control() == CLOSE) {
    P4OUT ^= BIT7;
    __delay_cycles(1000);
  }

  STATUS_ANT[(antenna - 1)] = OPEN;
  P4OUT = BIT7;
}

void deploy_sequencial_ant(void) {
  if (STATUS_ANT[0] || STATUS_ANT[1] || STATUS_ANT[2] || STATUS_ANT[3]) {
    deploy_ant(1);
    deploy_ant(2);
    deploy_ant(3);
    deploy_ant(4);
  }
}
