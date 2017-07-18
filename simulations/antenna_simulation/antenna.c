#include "antenna.h"

uint8_t state = STATE_DISARM;

// This function setup the antenna pins for simulation purposes.
void ant_setup(void) {

    BIT_SET(LED_ARM_DIR, LED_ARM_PIN);
    BIT_CLEAR(LED_ARM_OUT, LED_ARM_PIN);

    BIT_SET(LED_1_DIR, LED_1_PIN);
    BIT_SET(LED_2_DIR, LED_2_PIN);
    BIT_SET(LED_3_DIR, LED_3_PIN);
    BIT_SET(LED_4_DIR, LED_4_PIN);

    BIT_CLEAR(LED_1_OUT, LED_1_PIN);
    BIT_CLEAR(LED_2_OUT, LED_2_PIN);
    BIT_CLEAR(LED_3_OUT, LED_3_PIN);
    BIT_CLEAR(LED_4_OUT, LED_4_PIN);

    BIT_CLEAR(SWITCH_1_DIR, SWITCH_1_PIN);
    BIT_SET(SWITCH_1_REN, SWITCH_1_PIN);
    BIT_SET(SWITCH_1_OUT, SWITCH_1_PIN);

    BIT_CLEAR(SWITCH_2_DIR, SWITCH_2_PIN);
    BIT_SET(SWITCH_2_REN, SWITCH_2_PIN);
    BIT_SET(SWITCH_2_OUT, SWITCH_2_PIN);

    BIT_CLEAR(SWITCH_3_DIR, SWITCH_3_PIN);
    BIT_SET(SWITCH_3_REN, SWITCH_3_PIN);
    BIT_SET(SWITCH_3_OUT, SWITCH_3_PIN);

    BIT_CLEAR(SWITCH_4_DIR, SWITCH_4_PIN);
    BIT_SET(SWITCH_4_REN, SWITCH_4_PIN);
    BIT_SET(SWITCH_4_OUT, SWITCH_4_PIN);
}

// This one simulates the antenna arming process
void ant_arm(void) {
    BIT_SET(LED_ARM_OUT, LED_ARM_PIN);
}

// Simulates the antenna disarming process
void ant_disarm(void) {
    BIT_CLEAR(LED_ARM_OUT, LED_ARM_PIN);
}

// This simulates the deployment of one of the antennas
void ant_deploy(uint8_t antenna, uint16_t time_limit) {

    uint16_t time = 0;

    switch (antenna) {
    case ANTENNA_1:
        while (BIT_READ(SWITCH_1_IN, SWITCH_1_PIN) != CLOSE) {
            BIT_SET(LED_1_OUT, LED_1_PIN);
            __delay_cycles(1000);
            time++;

            if (time >= SAFETY_TIME || time >= time_limit)
                break;

            if (command == DISARM) {
                ant_disarm();
                state = STATE_DISARM;
                break;
            }

            if (command == DEPLOY_CANCEL) {
                state = STATE_ARM;
                break;
            }
        }
        BIT_CLEAR(LED_1_OUT, LED_1_PIN);

    case ANTENNA_2:
        while (BIT_READ(SWITCH_2_IN, SWITCH_2_PIN) != CLOSE) {
            BIT_SET(LED_2_OUT, LED_2_PIN);
            __delay_cycles(1000);
            time++;

            if (time >= SAFETY_TIME || time >= time_limit)
                break;

            if (command == DISARM) {
                ant_disarm();
                state = STATE_DISARM;
                break;
            }

            if (command == DEPLOY_CANCEL) {
                state = STATE_ARM;
                break;
            }
        }
        BIT_CLEAR(LED_2_OUT, LED_2_PIN);
        break;

    case ANTENNA_3:
        while (BIT_READ(SWITCH_3_IN, SWITCH_3_PIN) != CLOSE) {
            BIT_SET(LED_3_OUT, LED_3_PIN);
            __delay_cycles(1000);
            time++;

            if (time >= SAFETY_TIME || time >= time_limit)
                break;

            if (command == DISARM) {
                ant_disarm();
                state = STATE_DISARM;
                break;
            }

            if (command == DEPLOY_CANCEL) {
                state = STATE_ARM;
                break;
            }
        }
        BIT_CLEAR(LED_3_OUT, LED_3_PIN);
        break;

    case ANTENNA_4:
        while (BIT_READ(SWITCH_4_IN, SWITCH_4_PIN) != CLOSE) {
            BIT_SET(LED_4_OUT, LED_4_PIN);
            __delay_cycles(1000);
            time++;

            if (time >= SAFETY_TIME || time >= time_limit)
                break;

            if (command == DISARM) {
                ant_disarm();
                state = STATE_DISARM;
                break;
            }

            if (command == DEPLOY_CANCEL) {
                state = STATE_ARM;
                break;
            }
        }
        BIT_CLEAR(LED_4_OUT, LED_4_PIN);
        break;
    }
}

// This simulates the sequential deployment of all antennas
void ant_deploy_sequencial(uint16_t time_limit) {

    ant_deploy(ANTENNA_1, time_limit);
    if (state == STATE_DISARM || state == STATE_ARM)
        return;
    ant_deploy(ANTENNA_2, time_limit);
    if (state == STATE_DISARM || state == STATE_ARM)
        return;
    ant_deploy(ANTENNA_3, time_limit);
    if (state == STATE_DISARM || state == STATE_ARM)
        return;
    ant_deploy(ANTENNA_4, time_limit);

}

void switch_command(void) {

    switch (command) {

    case RESET:
        WDTCTL = 0;

    case DISARM:
        ant_disarm();
        state = STATE_DISARM;
        break;

    case ARM:
        if (state == STATE_DISARM) {
            ant_arm();
            state = STATE_ARM;
        }
        break;

    case DEPLOY_ANT_1:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_1;
            ant_deploy(ANTENNA_1, 10000);
            if (state == STATE_DISARM) {
                break;
            }
            state = STATE_ARM;
        }
        break;

    case DEPLOY_ANT_2:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_2;
            ant_deploy(ANTENNA_2, 10000);
            if (state == STATE_DISARM) {
                break;
            }
            state = STATE_ARM;
        }
        break;

    case DEPLOY_ANT_3:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_3;
            ant_deploy(ANTENNA_3, 10000);
            if (state == STATE_DISARM) {
                break;
            }
            state = STATE_ARM;
        }
        break;

    case DEPLOY_ANT_4:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_4;
            ant_deploy(ANTENNA_4, 10000);
            if (state == STATE_DISARM) {
                break;
            }
            state = STATE_ARM;
        }
        break;

    case DEPLOY_SEQUENCIAL:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_SEQ;
            ant_deploy_sequencial(10000);
            if (state == STATE_ARM) {
                break;
            }
            state = STATE_DISARM;
        }
        break;

    default:
        break;
    }
}
