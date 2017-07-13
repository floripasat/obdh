#include "antenna.h"

uint8_t STATUS_DEPLOY[4];
volatile uint8_t ARM_STATUS = 0;

void ant_setup(void) {

    //Represent the burn-wire system

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

void ant_arm(void) {

    volatile uint8_t SWITCH_STATUS_1, SWITCH_STATUS_2, SWITCH_STATUS_3, SWITCH_STATUS_4;

    SWITCH_STATUS_1 = BIT_READ(SWITCH_1_IN, SWITCH_1_PIN);
    SWITCH_STATUS_2 = BIT_READ(SWITCH_2_IN, SWITCH_2_PIN);
    SWITCH_STATUS_3 = BIT_READ(SWITCH_3_IN, SWITCH_3_PIN);
    SWITCH_STATUS_4 = BIT_READ(SWITCH_4_IN, SWITCH_4_PIN);

    if (ARM_STATUS == DISARMED) {
        if (SWITCH_STATUS_1 || SWITCH_STATUS_2 || SWITCH_STATUS_3 || SWITCH_STATUS_4) {
            ARM_STATUS = ARMED;
            BIT_SET(LED_ARM_OUT, LED_ARM_PIN);
        }
    }
}

void ant_disarm(void) {
    if (ARM_STATUS == ARMED) {
        ARM_STATUS = DISARMED;
        BIT_CLEAR(LED_ARM_OUT, LED_ARM_PIN);
    }
}

void ant_deploy(uint8_t antenna) {

    uint8_t time = 0;

    if (ARM_STATUS == ARMED) {
        switch (antenna) {
        case ANTENNA_1:
            while (BIT_READ(SWITCH_1_IN, SWITCH_1_PIN) != CLOSE) {
                BIT_TOGGLE(LED_1_OUT, LED_1_PIN);
                __delay_cycles(1000000);
                time++;

                if (time >= SAFETY_TIME) {
                    STATUS_DEPLOY[0] = CLOSE;
                    BIT_CLEAR(LED_1_OUT, LED_1_PIN);
                    break;
                }
            }
            if (time < SAFETY_TIME) {
                STATUS_DEPLOY[0] = OPEN;
                BIT_SET(LED_1_OUT, LED_1_PIN);
            }
            break;

        case ANTENNA_2:
            while (BIT_READ(SWITCH_2_IN, SWITCH_2_PIN) != CLOSE) {
                BIT_TOGGLE(LED_2_OUT, LED_2_PIN);
                __delay_cycles(1000000);
                time++;

                if (time >= SAFETY_TIME) {
                    STATUS_DEPLOY[1] = CLOSE;
                    BIT_CLEAR(LED_2_OUT, LED_2_PIN);
                    break;
                }
            }
            if (time < SAFETY_TIME) {
                STATUS_DEPLOY[1] = OPEN;
                BIT_SET(LED_2_OUT, LED_2_PIN);
            }
            break;

        case ANTENNA_3:
            while (BIT_READ(SWITCH_3_IN, SWITCH_3_PIN) != CLOSE) {
                BIT_TOGGLE(LED_3_OUT, LED_3_PIN);
                __delay_cycles(1000000);
                time++;

                if (time >= SAFETY_TIME) {
                    STATUS_DEPLOY[2] = CLOSE;
                    BIT_CLEAR(LED_3_OUT, LED_3_PIN);
                    break;
                }
            }
            if (time < SAFETY_TIME) {
                STATUS_DEPLOY[2] = OPEN;
                BIT_SET(LED_3_OUT, LED_3_PIN);
            }
            break;

        case ANTENNA_4:
            while (BIT_READ(SWITCH_4_IN, SWITCH_4_PIN) != CLOSE) {
                BIT_TOGGLE(LED_4_OUT, LED_4_PIN);
                __delay_cycles(1000000);
                time++;

                if (time >= SAFETY_TIME) {
                    STATUS_DEPLOY[3] = CLOSE;
                    BIT_CLEAR(LED_4_OUT, LED_4_PIN);
                    break;
                }
            }
            if (time < SAFETY_TIME) {
                STATUS_DEPLOY[3] = OPEN;
                BIT_SET(LED_4_OUT, LED_4_PIN);
            }
            break;
        }
    }
}

void ant_deploy_sequencial(void) {

    if (!(STATUS_DEPLOY[0] || STATUS_DEPLOY[1] || STATUS_DEPLOY[2] || STATUS_DEPLOY[3])) {
        ant_deploy(ANTENNA_1);
        ant_deploy(ANTENNA_2);
        ant_deploy(ANTENNA_3);
        ant_deploy(ANTENNA_4);
    }
}

void ant_switch_command(uint8_t command) {

    __disable_interrupt();

    switch (command) {
    case ARMING:
        ant_arm();
        break;

    case DISARMING:
        ant_disarm();
        break;

    case DEPLOY_ANT_1:
        ant_deploy(ANTENNA_1);
        break;

    case DEPLOY_ANT_2:
        ant_deploy(ANTENNA_2);
        break;

    case DEPLOY_ANT_3:
        ant_deploy(ANTENNA_3);
        break;

    case DEPLOY_ANT_4:
        ant_deploy(ANTENNA_4);
        break;

    case DEPLOY_SEQUENCIAL:
        ant_deploy_sequencial();
        break;

    default:
        break;
    }
    __enable_interrupt();
}

