#include "antenna.h"

uint8_t STATUS_ANT[4];
uint8_t armed = 0;

void arming_ant(void) {
    if (armed == 0) {
        if (switch_control() == CLOSE) {
            armed = 1;
            P1OUT |= BIT0;
        }
    }
}

void disarming_ant(void) {
    if (armed == 1) {
        armed = 0;
        P1OUT &= ~BIT0;
    }
}

void deploy_ant(uint8_t antenna) {

    uint8_t safety_time = 30;
    uint8_t time = 0;

    if (armed == 1) {
        while (switch_control() == CLOSE) {
            P4OUT ^= BIT7;
            __delay_cycles(1000000);
            time++;

            if (time >= safety_time) {
                STATUS_ANT[(antenna - 1)] = CLOSE;
                P4OUT &= ~BIT7;
                break;
            }
        }
        if (time < safety_time) {
            STATUS_ANT[(antenna - 1)] = OPEN;
            P4OUT |= BIT7;
        }
    }
}


void deploy_sequencial_ant(void) {

    if (!(STATUS_ANT[0] || STATUS_ANT[1] || STATUS_ANT[2] || STATUS_ANT[3])) {
        deploy_ant(1);
        deploy_ant(2);
        deploy_ant(3);
        deploy_ant(4);
    }
}

void switch_command(uint8_t command) {

    __disable_interrupt();

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
    __enable_interrupt();
}

