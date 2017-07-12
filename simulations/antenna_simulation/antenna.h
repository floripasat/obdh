#include <msp430.h>
#include <stdint.h>

#define ARMING				0xAD
#define DISARMING			0xAC
#define DEPLOY_ANT_1		0xA1
#define DEPLOY_ANT_2		0xA2
#define DEPLOY_ANT_3		0xA3
#define DEPLOY_ANT_4		0xA4
#define DEPLOY_SEQUENCIAL	0xA5

#define CLOSE               0
#define OPEN                1
#define switch_control()    !(P1IN & BIT1)

void arming_ant(void);
void disarming_ant(void);
void deploy_ant(uint8_t antenna);
void deploy_sequencial_ant(void);
void switch_command(uint8_t command);

