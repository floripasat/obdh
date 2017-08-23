/*
 * antenna.h
 *
 *  Created on: 10 de jul de 2017
 *      Author: André
 */

#ifndef ANTENNA_H_
#define ANTENNA_H_

#include <msp430.h>
#include <stdint.h>
#include "misc_def.h"
#include "temp_data.h"

// States
#define STATE_ARM                   1
#define STATE_DISARM                2
#define STATE_DEPLOY_1              3
#define STATE_DEPLOY_2              4
#define STATE_DEPLOY_3              5
#define STATE_DEPLOY_4              6
#define STATE_DEPLOY_1_OVERRIDE     7
#define STATE_DEPLOY_2_OVERRIDE     8
#define STATE_DEPLOY_3_OVERRIDE     9
#define STATE_DEPLOY_4_OVERRIDE     10
#define STATE_DEPLOY_SEQ            11

// Commands
#define RESET                       0xAA
#define ARM				            0xAD
#define DISARM			            0xAC
#define DEPLOY_ANT_1		        0xA1
#define DEPLOY_ANT_2		        0xA2
#define DEPLOY_ANT_3		        0xA3
#define DEPLOY_ANT_4		        0xA4
#define DEPLOY_SEQUENCIAL	        0xA5
#define DEPLOY_ANT_1_OVERRIDE       0xBA
#define DEPLOY_ANT_2_OVERRIDE       0xBB
#define DEPLOY_ANT_3_OVERRIDE       0xBC
#define DEPLOY_ANT_4_OVERRIDE       0xBD
#define DEPLOY_CANCEL               0xA9
#define MEASURE_TEMPERATURE         0xC0
#define REPORT_DEPLOY_COUNTER_1     0xB0
#define REPORT_DEPLOY_COUNTER_2     0xB1
#define REPORT_DEPLOY_COUNTER_3     0xB2
#define REPORT_DEPLOY_COUNTER_4     0xB3
#define REPORT_DEPLOY_TIMER_1       0xB4
#define REPORT_DEPLOY_TIMER_2       0xB5
#define REPORT_DEPLOY_TIMER_3       0xB6
#define REPORT_DEPLOY_TIMER_4       0xB7
#define REPORT_DEPLOY_STATUS        0xC3

// Report status definitions
#define STATUS_ARMED                   1
#define STATUS_DISARMED                0
#define STATUS_NOT_DEPLOYED            1
#define STATUS_DEPLOYED                0
#define STATUS_CAUSE_TIMEOUT           1
#define STATUS_CAUSE_OTHER             0
#define STATUS_BURNING_ACTIVE          1
#define STATUS_BURNING_NOT_ACTIVE      0
#define STATUS_BURNING_INDP_ACTIVE     1
#define STATUS_BURNING_INDP_NOT_ACTIVE 0
#define STATUS_IGNORING                1
#define STATUS_NOT_IGNORING            0

// Antenna hardware definitions
#define LED_ARM_DIR     P8DIR
#define LED_ARM_OUT     P8OUT
#define LED_ARM_PIN     BIT1

#define LED_1_DIR       P2DIR
#define LED_1_OUT       P2OUT
#define LED_1_PIN       BIT3

#define LED_2_DIR       P2DIR
#define LED_2_OUT       P2OUT
#define LED_2_PIN       BIT6

#define LED_3_DIR       P8DIR
#define LED_3_OUT       P8OUT
#define LED_3_PIN       BIT2

#define LED_4_DIR       P3DIR
#define LED_4_OUT       P3OUT
#define LED_4_PIN       BIT7

#define SWITCH_1_DIR    P4DIR
#define SWITCH_1_IN     P4IN
#define SWITCH_1_REN    P4REN
#define SWITCH_1_OUT    P4OUT
#define SWITCH_1_PIN    BIT1

#define SWITCH_2_DIR    P4DIR
#define SWITCH_2_IN     P4IN
#define SWITCH_2_REN    P4REN
#define SWITCH_2_OUT    P4OUT
#define SWITCH_2_PIN    BIT2

#define SWITCH_3_DIR    P2DIR
#define SWITCH_3_IN     P2IN
#define SWITCH_3_REN    P2REN
#define SWITCH_3_OUT    P2OUT
#define SWITCH_3_PIN    BIT7

#define SWITCH_4_DIR    P3DIR
#define SWITCH_4_IN     P3IN
#define SWITCH_4_REN    P3REN
#define SWITCH_4_OUT    P3OUT
#define SWITCH_4_PIN    BIT2

// Software Abstraction
#define ARMED               1
#define DISARMED            0

#define NOT_DEPLOYED        0
#define DEPLOYED            1
#define TIMEOUT             2

#define OPEN                0
#define CLOSE               1

#define OVERRIDE            1

#define ANTENNA_1           1
#define ANTENNA_2           2
#define ANTENNA_3           3
#define ANTENNA_4           4

// I2C definitions
#define I2C_FLAG            UCB0IFG
#define I2C_START_FLAG      UCSTTIFG
#define I2C_STOP_FLAG       UCSTPIFG
#define I2C_RX_FLAG         UCRXIFG
#define I2C_TX_FLAG         UCTXIFG
#define I2C_RX_BUFFER       UCB0RXBUF
#define I2C_TX_BUFFER       UCB0TXBUF

#define STEP_IN_1_S         3200
#define PERIOD              100//1000000 / STEP_IN_1_S
#define SAFETY_TIME_IN_CYCLES    96000

// Commands functionality
void ant_setup(void);
void ant_arm(void);
void ant_disarm(void);
uint8_t ant_deploy(uint8_t antenna, uint32_t time_limit, uint32_t time);
void ant_deploy_sequencial(uint32_t time_limit, uint32_t* time);
void run(void);

#endif /* ANTENNA_H_ */
