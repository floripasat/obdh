#include "i2c.h"

uint8_t response[2];
uint8_t *ptr_response;
uint8_t *ptr_command;

void i2c_setup(void) {

    BIT_SET(SDA_REN, SDA_PIN);                                  // Enable resistor on SDA.
    BIT_SET(SCL_REN, SCL_PIN);                                  // Enable resistor on SCL.

    BIT_SET(SDA_OUT, SDA_PIN);                                  // Set resistor to pull-up on SDA.
    BIT_SET(SCL_OUT, SCL_PIN);                                  // Set resistor to pull-up on SCL.

    BIT_SET(SDA_SEL, SDA_PIN);                                  // Assign I2C SDA to USCI_B0.
    BIT_SET(SCL_SEL, SCL_PIN);                                  // Assign I2C SCL to USCI_B0.

    BIT_SET(I2C_CONTROL_1, I2C_SW_RESET);                       // Enable SW reset.
    BIT_SET(I2C_CONTROL_0, I2C_SLAVE_MODE);                     // I2C Slave.
    BIT_SET(I2C_CONTROL_0, I2C_SYNC_MODE);                      // Synchronous mode.
    BIT_SET(I2C_OWN_ADDRESS, SLAVE_ADDRESS_A);
    BIT_CLEAR(I2C_CONTROL_1, I2C_SW_RESET);                     // Clear SW reset, resume operation.

    I2C_EN_INTERRUPT |= UCSTPIE | UCSTTIE | UCRXIE | UCTXIE;    // Enable STT, STP, TX & RX interrupt.
    __bis_SR_register(GIE);

    response[0] = 0x0F;                                         //0xFF is the default value.
    response[1] = 0x0F;
}

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
    switch(__even_in_range(UCB0IV,12))
    {
    case  0: break;                                 // Vector  0: No interrupts.
    case  2: break;                                 // Vector  2: ALIFG.
    case  4: break;                                 // Vector  4: NACKIFG.

    case  6:                                        // Vector  6: STTIFG.
        ptr_response = (uint8_t *)response;
        ptr_command  = (uint8_t *)command;
        BIT_CLEAR(I2C_FLAG, I2C_START);
        break;

    case  8:                                        // Vector  8: STPIFG.
        response[0] = 0x0F;
        response[1] = 0x0F;
        BIT_CLEAR(I2C_FLAG, I2C_STOP);
        break;

    case 10:                                        // Vector 10: RXIFG.
        *ptr_command++ = I2C_RX_BUFFER;

        switch (command[0]) {                          // Make the transition of states from the received command and refresh the response variable when requested.
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
            response[0] = (temperature && 0x0300) >> 8;
            response[1] = temperature && 0xFF;
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
            response[0] = (deploy_timer_1 && 0xFF00) >> 8;
            response[1] = (deploy_timer_1 && 0xFF);
            break;

        case REPORT_DEPLOY_TIMER_2:
            response[0] = (deploy_timer_2 && 0xFF00) >> 8;
            response[1] = (deploy_timer_2 && 0xFF);
            break;

        case REPORT_DEPLOY_TIMER_3:
            response[0] = (deploy_timer_3 && 0xFF00) >> 8;
            response[1] = (deploy_timer_3 && 0xFF);
            break;

        case REPORT_DEPLOY_TIMER_4:
            response[0] = (deploy_timer_4 && 0xFF00) >> 8;
            response[1] = (deploy_timer_4 && 0xFF);
            break;

        case REPORT_DEPLOY_STATUS:
            response[0] = (*ptr_report_status && 0xFF00) >> 8;
            response[1] = (*ptr_report_status && 0xFF);

        default:
            break;
        }

        case 12:                                  	// Vector 12: TXIFG
            I2C_TX_BUFFER = *ptr_response++;        // Send the response from a command when requested.
            break;

        default: break;
    }
}
