/*
 * antenna.c
 *
 *  Created on: 10 de jul de 2017
 *      Author: André
 */

#include "antenna.h"

typedef struct {                            // This struct is inverted because the MSP memory allocation
    unsigned armed              :1;
    unsigned antenna_burning_4  :1;
    unsigned antenna_cause_4    :1;
    unsigned deploy_status_4    :1;
    unsigned independent_burn   :1;
    unsigned antenna_burning_3  :1;
    unsigned antenna_cause_3    :1;
    unsigned deploy_status_3    :1;
    unsigned ignoring_switches  :1;
    unsigned antenna_burning_2  :1;
    unsigned antenna_cause_2    :1;
    unsigned deploy_status_2    :1;
    unsigned null               :1;
    unsigned antenna_burning_1  :1;
    unsigned antenna_cause_1    :1;
    unsigned deploy_status_1    :1;
} report_t;

report_t report_status;                         // Store the 16 bits defined in report_t struct.
uint16_t *ptr_report_status;                    // This is used for convert the report_t as uint16_t.
uint8_t response[2];                            // The response that will be send when requested from a command.
uint8_t *ptr_response;                          // Pointer for I2C purposes (initializate the response[] in the first position).
uint8_t *ptr_command;                           // Receive the commands by the Rx Buffer of I2C.

uint16_t temperature;                           // Store the temperature data.

// Store the amount of time that the burn-wire system was active.
uint32_t deploy_timer_1, deploy_timer_2, deploy_timer_3, deploy_timer_4;
// Store the amount of times the deployment system was active. 
uint8_t deploy_counter_1, deploy_counter_2, deploy_counter_3, deploy_counter_4;
uint32_t time_to_burn;
uint8_t deploy_mode;                        // Choose the deploy mode (Override/Normal).
uint8_t ant_deploy_run;                     // Store the function return to be used in if statements.
uint8_t first_activation = 0;               // This ensure that a deploy doesn't happen before other finishes.
static uint8_t antenna_select = ANTENNA_1;  // Select the next antenna for actually happen the sequencial deploy.

uint8_t command[2] = {0, 0};                
uint8_t state = STATE_DISARM;                

void get_report_status( uint8_t *response) {
    report_status.deploy_status_1 = BIT_READ(SWITCH_1_IN, SWITCH_1_PIN);
    report_status.deploy_status_2 = BIT_READ(SWITCH_2_IN, SWITCH_2_PIN);
    report_status.deploy_status_3 = BIT_READ(SWITCH_3_IN, SWITCH_3_PIN);
    report_status.deploy_status_4 = BIT_READ(SWITCH_4_IN, SWITCH_4_PIN);
    response[1] = (*ptr_report_status & 0xFF00) >> 8;
    response[0] = (*ptr_report_status & 0xFF);
}
// This function setup the antenna pins for simulation purposes.
void ant_setup(void) {

    // Armed/Desarmed led setup.
    BIT_SET(LED_ARM_DIR, LED_ARM_PIN);          
    BIT_CLEAR(LED_ARM_OUT, LED_ARM_PIN);

    // Antennas setup.
    BIT_SET(LED_1_DIR, LED_1_PIN); 
    BIT_SET(LED_2_DIR, LED_2_PIN);
    BIT_SET(LED_3_DIR, LED_3_PIN);
    BIT_SET(LED_4_DIR, LED_4_PIN);

    BIT_CLEAR(LED_1_OUT, LED_1_PIN);            
    BIT_CLEAR(LED_2_OUT, LED_2_PIN);
    BIT_CLEAR(LED_3_OUT, LED_3_PIN);
    BIT_CLEAR(LED_4_OUT, LED_4_PIN);

    // Switches setup.
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

    // Initial values for status report. 
    report_status.armed = STATUS_DISARMED;
    report_status.deploy_status_1 = STATUS_NOT_DEPLOYED;
    report_status.deploy_status_2 = STATUS_NOT_DEPLOYED;
    report_status.deploy_status_3 = STATUS_NOT_DEPLOYED;
    report_status.deploy_status_4 = STATUS_NOT_DEPLOYED;
    report_status.null = 0;

    deploy_timer_1 = 0;
    deploy_timer_2 = 0;
    deploy_timer_3 = 0;
    deploy_timer_4 = 0;
    deploy_counter_1 = 0;
    deploy_counter_2 = 0;
    deploy_counter_3 = 0;
    deploy_counter_4 = 0;

    response[0] = 0x88;                                         // 0x88 is the default value.
    response[1] = 0x88;                                         // It was choose for debug purposes.
}

// Antenna arming process. Refresh the ARM_LED and ensure that the ANTENNA_LEDS are turn off.
void ant_arm(void) {
    BIT_SET(LED_ARM_OUT, LED_ARM_PIN);
    BIT_CLEAR(LED_1_OUT, LED_1_PIN);
    BIT_CLEAR(LED_2_OUT, LED_2_PIN);
    BIT_CLEAR(LED_3_OUT, LED_3_PIN);
    BIT_CLEAR(LED_4_OUT, LED_4_PIN);
}

// Antenna disarming process. Does a similar function that the ant_arm().
void ant_disarm(void) {
    BIT_CLEAR(LED_ARM_OUT, LED_ARM_PIN);
    BIT_CLEAR(LED_1_OUT, LED_1_PIN);
    BIT_CLEAR(LED_2_OUT, LED_2_PIN);
    BIT_CLEAR(LED_3_OUT, LED_3_PIN);
    BIT_CLEAR(LED_4_OUT, LED_4_PIN);
}

// Deployment of one of the antennas.
uint8_t ant_deploy(uint8_t antenna, uint32_t time_limit, volatile uint32_t time) {

    uint8_t *LED_OUT, *SWITCH_IN;       // Store the Register address. 
    uint16_t LED_PIN, SWITCH_PIN;       // Store the Pin number.

    switch (antenna) {                  // Choose which antenna deploys.
    case ANTENNA_1:
        SWITCH_IN  = (uint8_t *) &SWITCH_1_IN;
        SWITCH_PIN = SWITCH_1_PIN;
        LED_OUT    = (uint8_t *) &LED_1_OUT;
        LED_PIN    = LED_1_PIN;

        deploy_timer_1++;
        break;

    case ANTENNA_2:
        SWITCH_IN  = (uint8_t *) &SWITCH_2_IN;
        SWITCH_PIN = SWITCH_2_PIN;
        LED_OUT    = (uint8_t *) &LED_2_OUT;
        LED_PIN    = LED_2_PIN;

        deploy_timer_2++;
        break;

    case ANTENNA_3:
        SWITCH_IN  = (uint8_t *) &SWITCH_3_IN;
        SWITCH_PIN = SWITCH_3_PIN;
        LED_OUT    = (uint8_t *) &LED_3_OUT;
        LED_PIN    = LED_3_PIN;

        deploy_timer_3++;
        break;

    case ANTENNA_4:
        SWITCH_IN  = (uint8_t *) &SWITCH_4_IN;
        SWITCH_PIN = SWITCH_4_PIN;
        LED_OUT    = (uint8_t *) &LED_4_OUT;
        LED_PIN    = LED_4_PIN;

        deploy_timer_4++;
        break;

    default:
        break;
    }

    if (deploy_mode != OVERRIDE) {        
        // Checks the switch condition, if it's open, stops the burn-wire system.
        if (BIT_READ(*SWITCH_IN, SWITCH_PIN) == OPEN) {
            BIT_CLEAR(*LED_OUT, LED_PIN);
            return DEPLOYED;
        }
    }

    // Checks whether the safety time and the time limit are exceeded, in case true stops the burn-wire system.
    if (
            (time   >= (SAFETY_TIME_IN_CYCLES) )
            ||
            (
                 (time   >=  time_limit)
                 &&
                 (time_limit != 0)
            )
        ) {
        BIT_CLEAR(*LED_OUT, LED_PIN);
        return TIMEOUT;
    }
    else {
        // If the above conditions aren't true, then deploy continues.
        BIT_SET(*LED_OUT, LED_PIN);
        return NOT_DEPLOYED;
    }
}

// Makes the sequential deploy of all antennas, for this it calls the independent deployment function.
void ant_deploy_sequencial(uint32_t time_limit, uint32_t* time) {

    switch (antenna_select) {
    case ANTENNA_1:
        if( first_activation == 0) {            // Ensure that a deploy doesn't happen before other finishes.
            first_activation = 1;
            deploy_counter_1++;                 // Considers more one deploy try.
        }

        ant_deploy_run = ant_deploy(ANTENNA_1, time_limit, *time);

        // Checks the return of ant_deploy(), if deploy is finished, the system of the next antenna is selected.
        if (ant_deploy_run == TIMEOUT) {
            report_status.antenna_cause_1 = STATUS_CAUSE_TIMEOUT;
            report_status.antenna_burning_1 = STATUS_BURNING_NOT_ACTIVE;
            *time = 0;
            antenna_select = ANTENNA_2;
            first_activation = 0;
        }
        if (ant_deploy_run == DEPLOYED) {
            report_status.antenna_cause_1 = STATUS_CAUSE_OTHER;
            report_status.antenna_burning_1 = STATUS_BURNING_NOT_ACTIVE;
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

        ant_deploy_run = ant_deploy(ANTENNA_2, time_limit, *time);

        if (ant_deploy_run == TIMEOUT) {
            report_status.antenna_cause_2 = STATUS_CAUSE_TIMEOUT;
            report_status.antenna_burning_2 = STATUS_BURNING_NOT_ACTIVE;
            *time = 0;
            antenna_select = ANTENNA_3;
            first_activation = 0;
        }
        if (ant_deploy_run == DEPLOYED) {
            report_status.antenna_cause_2 = STATUS_CAUSE_OTHER;
            report_status.antenna_burning_2 = STATUS_BURNING_NOT_ACTIVE;
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

        ant_deploy_run = ant_deploy(ANTENNA_3, time_limit, *time);

        if (ant_deploy_run == TIMEOUT) {
            report_status.antenna_cause_3 = STATUS_CAUSE_TIMEOUT;
            report_status.antenna_burning_3 = STATUS_BURNING_NOT_ACTIVE;
            *time = 0;
            antenna_select = ANTENNA_4;
            first_activation = 0;
        }
        if (ant_deploy_run == DEPLOYED) {
            report_status.antenna_cause_3 = STATUS_CAUSE_OTHER;
            report_status.antenna_burning_3 = STATUS_BURNING_NOT_ACTIVE;
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

        ant_deploy_run = ant_deploy(ANTENNA_4, time_limit, *time);

        if (ant_deploy_run == TIMEOUT) {
            report_status.antenna_cause_4 = STATUS_CAUSE_TIMEOUT;
            report_status.antenna_burning_4 = STATUS_BURNING_NOT_ACTIVE;
            *time = 0;
            state = STATE_ARM;
            first_activation = 0;
        }
        if (ant_deploy_run == DEPLOYED) {
            report_status.antenna_cause_4 = STATUS_CAUSE_OTHER;
            report_status.antenna_burning_4 = STATUS_BURNING_NOT_ACTIVE;
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

/*
 * This switch works like a FSM (Finite State Machine), so makes
 * the transition of states from the received command and
 * refresh some variables when requested.
 *
 * Commands List:
 *    RESET                       Reset the microcontroller.
 *    ARM/DISARM                  Arm and disarm the antennas system.
 *    DEPLOY_ANT_X                Active a individual burn-wire system.
 *    DEPLOY_ANT_X_OVERRIDE       Active a individual burn-wire system with override.
 *    DEPLOY_SEQUENCIAL           Active the sequencial burn-wire system.
 *    DEPLOY_CANCEL               Cancel a current deploy.
 *    MEASURE_TEMPERATURE         Sends the microcontroller temperature.
 *    REPORT_DEPLOY_COUNTER_X     Sends the amount of times the deployment system was active.
 *    REPORT_DEPLOY_TIMER_X       Sends the amount of time the burn-wire was active.
 *    REPORT_DEPLOY_STATUS        Reports the current antenna status.
 */

void command_read(void) {

    switch (command[0]) {
    case DEPLOY_CANCEL:
        state = STATE_ARM;
        break;

    case RESET:
        WDTCTL = 0;
        break;

    case DISARM:
        state = STATE_DISARM;
        break;

    case ARM:
        if (state == STATE_DISARM) {
            state = STATE_ARM;
        }
        break;

    case DEPLOY_ANT_1:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_1;
            deploy_counter_1++;
        }
        break;

    case DEPLOY_ANT_2:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_2;
            deploy_counter_2++;
        }
        break;

    case DEPLOY_ANT_3:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_3;
            deploy_counter_3++;
        }
        break;

    case DEPLOY_ANT_4:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_4;
            deploy_counter_4++;
        }
        break;

    case DEPLOY_ANT_1_OVERRIDE:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_1_OVERRIDE;
            deploy_counter_1++;
        }
        break;

    case DEPLOY_ANT_2_OVERRIDE:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_2_OVERRIDE;
            deploy_counter_2++;
        }
        break;

    case DEPLOY_ANT_3_OVERRIDE:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_3_OVERRIDE;
            deploy_counter_3++;
        }
        break;

    case DEPLOY_ANT_4_OVERRIDE:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_4_OVERRIDE;
            deploy_counter_4++;
        }
        break;

    case DEPLOY_SEQUENCIAL:
        if (state == STATE_ARM) {
            state = STATE_DEPLOY_SEQ;
        }
        break;

    case MEASURE_TEMPERATURE:
        response[0] = (temperature & 0x0300) >> 8;
        response[1] = temperature & 0xFF;
        break;

    case REPORT_DEPLOY_COUNTER_1:
        response[0] = deploy_counter_1;
        break;

    case REPORT_DEPLOY_COUNTER_2:
        response[0] = deploy_counter_2;
        break;

    case REPORT_DEPLOY_COUNTER_3:
        response[0] = deploy_counter_3;
        break;

    case REPORT_DEPLOY_COUNTER_4:
        response[0] = deploy_counter_4;
        break;

    case REPORT_DEPLOY_TIMER_1:
        deploy_timer_1 = deploy_timer_1;               // Convert to 50 milliseconds steps for simulation behavior purposes.
        response[0] = (deploy_timer_1 & 0xFF00) >> 8;
        response[1] = (deploy_timer_1 & 0xFF);
        break;

    case REPORT_DEPLOY_TIMER_2:
        deploy_timer_2 = deploy_timer_2;
        response[0] = (deploy_timer_2 & 0xFF00) >> 8;
        response[1] = (deploy_timer_2 & 0xFF);
        break;

    case REPORT_DEPLOY_TIMER_3:
        deploy_timer_3 = deploy_timer_3;
        response[0] = (deploy_timer_3 & 0xFF00) >> 8;
        response[1] = (deploy_timer_3 & 0xFF);
        break;

    case REPORT_DEPLOY_TIMER_4:
        deploy_timer_4 = deploy_timer_4;
        response[0] = (deploy_timer_4 & 0xFF00) >> 8;
        response[1] = (deploy_timer_4 & 0xFF);
        break;

    case REPORT_DEPLOY_STATUS:
        get_report_status(response);
        break;

    default:
        break;
    }
}

void ISR(void) {

    // Start
    if (I2C_FLAG & I2C_START_FLAG) {
        ptr_response = (uint8_t *)response;
        ptr_command  = (uint8_t *)command;
        BIT_CLEAR(I2C_FLAG, I2C_START_FLAG);
    }

    // RX
    if (I2C_FLAG & I2C_RX_FLAG) {
        *ptr_command++ = I2C_RX_BUFFER;         //Receive the commands from master.
        time_to_burn = (uint32_t)command[1] * STEP_IN_1_S;
        //command_read();
    }

    // TX
    if (I2C_FLAG & I2C_TX_FLAG) {
        I2C_TX_BUFFER = *ptr_response++;        // Send the response from a command when requested.

    }

    // Stop
    if(I2C_FLAG & I2C_STOP_FLAG) {
        BIT_CLEAR(I2C_FLAG, I2C_STOP_FLAG);
        response[0] = 0x88;
        response[1] = 0x88;


        command_read();
    }
}

// Functionality loop. This call the functions in the corresponding states.
void run(void) {

    uint32_t time = 0;                                  // This is the general time counter for the system.

    temperature  = temperature_read();                  // Get the temperature value.
    ptr_report_status = (uint16_t *) &report_status;    // Convert the report_t to uint16_t.

    while (1) {

        /*
         * This switch makes the antenna functionality by calling the functions associated with each state.
         *  
         * States List:
         *    STATE_DISARM                Disarm the antennas system.
         *    STATE_ARM                   Arm the antennas system and restore some varibles. Also, refresh the status.             
         *    STATE_DEPLOY_X              Calls the individual deploy function and update the report status bits.
         *    STATE_DEPLOY_X_OVERRIDE     Calls the individual deploy function with override and update the report status bits.
         *    STATE_DEPLOY_SEQUENCIAL     Calls the sequencial deploy function.
         */

        switch (state) {
        case STATE_DISARM:
            ant_disarm();
            report_status.armed = STATUS_DISARMED;
            report_status.antenna_burning_1 = STATUS_BURNING_NOT_ACTIVE;
            report_status.antenna_burning_2 = STATUS_BURNING_NOT_ACTIVE;
            report_status.antenna_burning_3 = STATUS_BURNING_NOT_ACTIVE;
            report_status.antenna_burning_4 = STATUS_BURNING_NOT_ACTIVE;
            report_status.ignoring_switches = STATUS_NOT_IGNORING;
            report_status.independent_burn = STATUS_BURNING_INDP_NOT_ACTIVE;
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
            report_status.ignoring_switches = STATUS_NOT_IGNORING;
            report_status.independent_burn = STATUS_BURNING_INDP_NOT_ACTIVE;
            break;

        case STATE_DEPLOY_1:
            ant_deploy_run = ant_deploy(ANTENNA_1, time_to_burn, time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_1 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_1 = STATUS_CAUSE_OTHER;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_1 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_NOT_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_2:
            ant_deploy_run = ant_deploy(ANTENNA_2, time_to_burn, time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_2 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_2 = STATUS_CAUSE_OTHER;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_2 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_NOT_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_3:
            ant_deploy_run = ant_deploy(ANTENNA_3, time_to_burn, time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_3 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_3 = STATUS_CAUSE_OTHER;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_3 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_NOT_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_4:
            ant_deploy_run = ant_deploy(ANTENNA_4, time_to_burn, time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_4 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            if (ant_deploy_run == DEPLOYED) {
                report_status.antenna_cause_4 = STATUS_CAUSE_OTHER;
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
            ant_deploy_run = ant_deploy(ANTENNA_1, time_to_burn, time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_1 = STATUS_CAUSE_TIMEOUT;
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
            ant_deploy_run = ant_deploy(ANTENNA_2, time_to_burn, time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_2 = STATUS_CAUSE_TIMEOUT;
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
            ant_deploy_run = ant_deploy(ANTENNA_3, time_to_burn, time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_3 = STATUS_CAUSE_TIMEOUT;
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
            ant_deploy_run = ant_deploy(ANTENNA_4, time_to_burn, time);

            if (ant_deploy_run == TIMEOUT) {
                report_status.antenna_cause_4 = STATUS_CAUSE_TIMEOUT;
                state = STATE_ARM;
            }
            else {
                report_status.antenna_burning_4 = STATUS_BURNING_ACTIVE;
                report_status.ignoring_switches = STATUS_IGNORING;
                report_status.independent_burn = STATUS_BURNING_INDP_ACTIVE;
            }
            break;

        case STATE_DEPLOY_SEQ:
            ant_deploy_sequencial(time_to_burn, &time);
            break;

        default:
            break;
        }

        ISR();

        __delay_cycles(PERIOD);
        time++;
    }

}
