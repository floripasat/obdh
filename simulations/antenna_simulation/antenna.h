#include <msp430.h>
#include <stdint.h>

uint8_t command;

//Macros
#define BIT_TOGGLE(REG, BIT)    (REG ^= BIT)        /**< macro that toggles a bit in a register */
#define BIT_SET(REG, BIT)       (REG |= BIT)        /**< macro that set a bit in a register */
#define BIT_CLEAR(REG, BIT)     (REG &= ~BIT)       /**< macro that clear a bit in a register */
#define BIT_READ(REG, BIT)      (REG & BIT)         /**< macro that read a bit in a register */

//States
#define STATE_ARM               1
#define STATE_DISARM            2
#define STATE_DEPLOY_1          3
#define STATE_DEPLOY_2          4
#define STATE_DEPLOY_3          5
#define STATE_DEPLOY_4          6
#define STATE_DEPLOY_SEQ        7

//Commands
#define RESET                   0xAA
#define ARM				        0xAD
#define DISARM			        0xAC
#define DEPLOY_ANT_1		    0xA1
#define DEPLOY_ANT_2		    0xA2
#define DEPLOY_ANT_3		    0xA3
#define DEPLOY_ANT_4		    0xA4
#define DEPLOY_SEQUENCIAL	    0xA5
#define DEPLOY_CANCEL           0xA9

//Software Setup
#define ARMED           1
#define DISARMED        0

#define OPEN            1
#define CLOSE           0

#define SAFETY_TIME     30000               // In milliseconds

#define ANTENNA_1       1
#define ANTENNA_2       2
#define ANTENNA_3       3
#define ANTENNA_4       4

//Hardware Setup
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


//Commands functionality
void ant_setup(void);
void ant_arm(void);
void ant_disarm(void);
void ant_deploy(uint8_t antenna, uint16_t time_limit);
void ant_deploy_sequencial(uint16_t time_limit);
void switch_command(void);

