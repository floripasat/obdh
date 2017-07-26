#include "antenna.h"

typedef struct {
    unsigned deploy_status_1    :1;
    unsigned antenna_cause_1    :1;
    unsigned antenna_burning_1  :1;
    unsigned null               :1;
    unsigned deploy_status_2    :1;
    unsigned antenna_cause_2    :1;
    unsigned antenna_burning_2  :1;
    unsigned ignoring_switches  :1;
    unsigned deploy_status_3    :1;
    unsigned antenna_cause_3    :1;
    unsigned antenna_burning_3  :1;
    unsigned independent_burn   :1;
    unsigned deploy_status_4    :1;
    unsigned antenna_cause_4    :1;
    unsigned antenna_burning_4  :1;
    unsigned armed              :1;
} report_t;

report_t report_status;
uint16_t *ptr_report_status;

uint16_t temperature;

uint16_t deploy_timer_1, deploy_timer_2, deploy_timer_3, deploy_timer_4;
uint8_t deploy_counter_1, deploy_counter_2, deploy_counter_3, deploy_counter_4;

uint8_t deploy_mode;
uint8_t ant_deploy_run;
uint8_t first_activation = 0;
static uint8_t antenna_select = ANTENNA_1;

uint8_t command[2];
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

    report_status.armed = STATUS_DISARMED;
    report_status.deploy_status_1 = STATUS_NOT_DEPLOYED;
    report_status.deploy_status_2 = STATUS_NOT_DEPLOYED;
    report_status.deploy_status_3 = STATUS_NOT_DEPLOYED;
    report_status.deploy_status_4 = STATUS_NOT_DEPLOYED;
    report_status.null = 0;
}

// Antenna arming process.
void ant_arm(void) {
    BIT_SET(LED_ARM_OUT, LED_ARM_PIN);
    BIT_CLEAR(LED_1_OUT, LED_1_PIN);
    BIT_CLEAR(LED_2_OUT, LED_2_PIN);
    BIT_CLEAR(LED_3_OUT, LED_3_PIN);
    BIT_CLEAR(LED_4_OUT, LED_4_PIN);
}

// Antenna disarming process.
void ant_disarm(void) {
    BIT_CLEAR(LED_ARM_OUT, LED_ARM_PIN);
    BIT_CLEAR(LED_1_OUT, LED_1_PIN);
    BIT_CLEAR(LED_2_OUT, LED_2_PIN);
    BIT_CLEAR(LED_3_OUT, LED_3_PIN);
    BIT_CLEAR(LED_4_OUT, LED_4_PIN);
}

// Deployment of one of the antennas.
uint8_t ant_deploy(uint8_t antenna, uint8_t time_limit_sec, volatile uint16_t time) {

    uint8_t *LED_OUT, *SWITCH_IN;
    uint16_t LED_PIN, SWITCH_PIN;

    switch (antenna) {
    case ANTENNA_1:
        SWITCH_IN  = &SWITCH_1_IN;
        SWITCH_PIN = SWITCH_1_PIN;
        LED_OUT    = &LED_1_OUT;
        LED_PIN    = LED_1_PIN;

        deploy_timer_1++;
        break;

    case ANTENNA_2:
        SWITCH_IN  = &SWITCH_2_IN;
        SWITCH_PIN = SWITCH_2_PIN;
        LED_OUT    = &LED_2_OUT;
        LED_PIN    = LED_2_PIN;

        deploy_timer_2++;
        break;

    case ANTENNA_3:
        SWITCH_IN  = &SWITCH_3_IN;
        SWITCH_PIN = SWITCH_3_PIN;
        LED_OUT    = &LED_3_OUT;
        LED_PIN    = LED_3_PIN;

        deploy_timer_3++;
        break;

    case ANTENNA_4:
        SWITCH_IN  = &SWITCH_4_IN;
        SWITCH_PIN = SWITCH_4_PIN;
        LED_OUT    = &LED_4_OUT;
        LED_PIN    = LED_4_PIN;

        deploy_timer_4++;
        break;

    default:
        break;
    }

    // Deploy with override
    if (deploy_mode == OVERRIDE) {
        if (time >= SAFETY_TIME || (time >= time_limit_milli() && (time_limit_milli() != 0))) {
            return TIMEOUT;
        }
        else {
            BIT_SET(*LED_OUT, LED_PIN);
            return NOT_DEPLOYED;
        }
    }

    //Deploy without override
    else {
        if (BIT_READ(*SWITCH_IN, SWITCH_PIN) == OPEN) {
            BIT_CLEAR(*LED_OUT, LED_PIN);
            return DEPLOYED;
        }
        else {
            if(time >= SAFETY_TIME || (time >= time_limit_milli() && (time_limit_milli() != 0))) {
                BIT_CLEAR(*LED_OUT, LED_PIN);
                return TIMEOUT;
            }
            BIT_SET(*LED_OUT, LED_PIN);
            return NOT_DEPLOYED;
        }
    }
}

// Sequential deployment of all antennas.
void ant_deploy_sequencial(uint8_t time_limit_sec, uint16_t* time) {

    switch (antenna_select) {
    case ANTENNA_1:
        if( first_activation == 0) {
            first_activation = 1;
            deploy_counter_1++;
        }

        ant_deploy_run = ant_deploy(ANTENNA_1, time_limit_milli(), *time);

        if (ant_deploy_run == TIMEOUT) {
            report_status.antenna_cause_1 = STATUS_CAUSE_TIMEOUT;
            *time = 0;
            antenna_select = ANTENNA_2;
            first_activation = 0;
        }
        if (ant_deploy_run == DEPLOYED) {
            report_status.antenna_cause_1 = STATUS_CAUSE_OTHER;
            report_status.deploy_status_1 = STATUS_DEPLOYED;
            *time = 0;
            antenna_select = ANTENNA_2;
            first_activation = 0;
        }
        else {
            report_status.antenna_burning_1 = STATUS_BURNING_ACTIVE;
            report_status.ignoring_switches = STATUS_NOT_IGNORING;
            report_status.independent_burn = STATUS_BURNING_INDP_NOT_ACTIVE;
        }
        break;

    case ANTENNA_2:
        if( first_activation == 0) {
            first_activation = 1;
            deploy_counter_2++;
        }

        ant_deploy_run = ant_deploy(ANTENNA_2, time_limit_milli(), *time);

        if (ant_deploy_run == TIMEOUT) {
            report_status.antenna_cause_2 = STATUS_CAUSE_TIMEOUT;
            *time = 0;
            antenna_select = ANTENNA_3;
            first_activation = 0;
        }
        if (ant_deploy_run == DEPLOYED) {
            report_status.antenna_cause_2 = STATUS_CAUSE_OTHER;
            report_status.deploy_status_2 = STATUS_DEPLOYED;
            *time = 0;
            antenna_select = ANTENNA_3;
            first_activation = 0;
        }
        else {
            report_status.antenna_burning_2 = STATUS_BURNING_ACTIVE;
            report_status.ignoring_switches = STATUS_NOT_IGNORING;
            report_status.independent_burn = STATUS_BURNING_INDP_NOT_ACTIVE;
        }
        break;

    case ANTENNA_3:
        if( first_activation == 0) {
            first_activation = 1;
            deploy_counter_3++;
        }

        ant_deploy_run = ant_deploy(ANTENNA_3, time_limit_milli(), *time);

        if (ant_deploy_run == TIMEOUT) {
            report_status.antenna_cause_3 = STATUS_CAUSE_TIMEOUT;
            *time = 0;
            antenna_select = ANTENNA_4;
            first_activation = 0;
        }
        if (ant_deploy_run == DEPLOYED) {
            report_status.antenna_cause_3 = STATUS_CAUSE_OTHER;
            report_status.deploy_status_3 = STATUS_DEPLOYED;
            *time = 0;
            antenna_select = ANTENNA_4;
            first_activation = 0;
        }
        else {
            report_status.antenna_burning_3 = STATUS_BURNING_ACTIVE;
            report_status.ignoring_switches = STATUS_NOT_IGNORING;
            report_status.independent_burn = STATUS_BURNING_INDP_NOT_ACTIVE;
        }
        break;

    case ANTENNA_4:
        if( first_activation == 0) {
            first_activation = 1;
            deploy_counter_4++;
        }

        ant_deploy_run = ant_deploy(ANTENNA_4, time_limit_milli(), *time);

        if (ant_deploy_run == TIMEOUT) {
            report_status.antenna_cause_4 = STATUS_CAUSE_TIMEOUT;
            *time = 0;
            state = STATE_ARM;
            first_activation = 0;
        }
        if (ant_deploy_run == DEPLOYED) {
            report_status.antenna_cause_4 = STATUS_CAUSE_OTHER;
            report_status.deploy_status_4 = STATUS_DEPLOYED;
            *time = 0;
            state = STATE_ARM;
            first_activation = 0;
        }
        else {
            report_status.antenna_burning_4 = STATUS_BURNING_ACTIVE;
            report_status.ignoring_switches = STATUS_NOT_IGNORING;
            report_status.independent_burn = STATUS_BURNING_INDP_NOT_ACTIVE;
        }
        break;
    }
}

// Functionality loop. This call the functions in the corresponding states.
void run(void) {

    uint16_t time = 0;

    temperature  = temperature_read();
    ptr_report_status = (uint16_t *)&report_status;

    while (1) {
        switch (state) {
        case STATE_DISARM:
            ant_disarm();
            report_status.armed = STATUS_DISARMED;
            break;

        case STATE_ARM:
            ant_arm();
            time = 0;
            deploy_mode = 0;
            first_activation = 0;
            antenna_select = ANTENNA_1;
            report_status.armed = STATUS_ARMED;
            report_status.antenna_burning_1 = STATUS_BURNING_NOT_ACTIVE;
            report_status.antenna_burning_2 = STATUS_BURNING_NOT_ACTIVE;
            report_status.antenna_burning_3 = STATUS_BURNING_NOT_ACTIVE;
            report_status.antenna_burning_4 = STATUS_BURNING_NOT_ACTIVE;
            report_status.independent_burn = STATUS_BURNING_INDP_NOT_ACTIVE;
            break;

        case STATE_DEPLOY_1:
            ant_deploy_run = ant_deploy(ANTENNA_1, command[1], time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_1 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_1 = STATUS_CAUSE_OTHER;
                report_status.deploy_status_1 = STATUS_DEPLOYED;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_1 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_NOT_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_2:
            ant_deploy_run = ant_deploy(ANTENNA_2, command[1], time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_2 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_2 = STATUS_CAUSE_OTHER;
                report_status.deploy_status_2 = STATUS_DEPLOYED;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_2 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_NOT_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_3:
            ant_deploy_run = ant_deploy(ANTENNA_3, command[1], time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_3 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_3 = STATUS_CAUSE_OTHER;
                report_status.deploy_status_3 = STATUS_DEPLOYED;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_3 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_NOT_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_4:
            ant_deploy_run = ant_deploy(ANTENNA_4, command[1], time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_4 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_4 = STATUS_CAUSE_OTHER;
                report_status.deploy_status_4 = STATUS_DEPLOYED;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_4 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_NOT_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_1_OVERRIDE:
            deploy_mode = OVERRIDE;
            ant_deploy_run = ant_deploy(ANTENNA_1, command[1], time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_1 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_1 = STATUS_CAUSE_OTHER;
                report_status.deploy_status_1 = STATUS_DEPLOYED;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_1 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_2_OVERRIDE:
            deploy_mode = OVERRIDE;
            ant_deploy_run = ant_deploy(ANTENNA_2, command[1], time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_2 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_2 = STATUS_CAUSE_OTHER;
                report_status.deploy_status_2 = STATUS_DEPLOYED;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_2 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_3_OVERRIDE:
            deploy_mode = OVERRIDE;
            ant_deploy_run = ant_deploy(ANTENNA_3, command[1], time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_3 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_3 = STATUS_CAUSE_OTHER;
                report_status.deploy_status_3 = STATUS_DEPLOYED;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_3 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_4_OVERRIDE:
            deploy_mode = OVERRIDE;
            ant_deploy_run = ant_deploy(ANTENNA_4, command[1], time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_4 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_4 = STATUS_CAUSE_OTHER;
                report_status.deploy_status_4 = STATUS_DEPLOYED;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_4 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_SEQ:
            ant_deploy_sequencial(command[1], &time);
            break;

        default:
            break;
        }
        __delay_cycles(1000);
        time++;
    }

}
