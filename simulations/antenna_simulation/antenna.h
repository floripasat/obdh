#include <msp430.h>
#include <stdint.h>

#define ARMING				10101101
#define DISARMING			10101100
#define DEPLOY_ANT_1		10100001
#define DEPLOY_ANT_2		10100010
#define DEPLOY_ANT_3		10100011
#define DEPLOY_ANT_4		10100100
#define DEPLOY_SEQUENCIAL	10100101

#define CLOSE               1
#define OPEN                0
#define switch_control()    (P1IN & BIT1)

void arming_ant(void);
void disarming_ant(void);
void deploy_ant(uint8_t antenna);
void deploy_sequencial_ant(void);

