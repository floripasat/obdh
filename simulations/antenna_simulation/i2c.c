#include "i2c.h"

void i2c_setup(void) {

    BIT_SET(SDA_REN, SDA_PIN);                          // Enable resistor on SDA
    BIT_SET(SCL_REN, SCL_PIN);                          // Enable resistor on SCL

    BIT_SET(SDA_OUT, SDA_PIN);                          // Set resistor to pull-up on SDA
    BIT_SET(SCL_OUT, SCL_PIN);                          // Set resistor to pull-up on SCL

    BIT_SET(SDA_SEL, SDA_PIN);                          // Assign I2C SDA to USCI_B0
    BIT_SET(SCL_SEL, SCL_PIN);                          // Assign I2C SCL to USCI_B0

    BIT_SET(I2C_CONTROL_1, I2C_SW_RESET);                       // Enable SW reset
    BIT_SET(I2C_CONTROL_0, I2C_SLAVE_MODE);                     // I2C Slave
    BIT_SET(I2C_CONTROL_0, I2C_SYNC_MODE);                      // Synchronous mode
    BIT_SET(I2C_OWN_ADDRESS, SLAVE_ADDRESS_A);
    BIT_CLEAR(I2C_CONTROL_1, I2C_SW_RESET);                     // Clear SW reset, resume operation

    I2C_EN_INTERRUPT |= UCSTPIE | UCSTTIE | UCRXIE | UCTXIE;    // Enable STT, STP, TX & RX interrupt
    __bis_SR_register(GIE);
}

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
    switch(__even_in_range(UCB0IV,12))
    {
    case  0: break;                                     // Vector  0: No interrupts
    case  2: break;                                     // Vector  2: ALIFG
    case  4: break;                                     // Vector  4: NACKIFG

    case  6:                                            // Vector  6: STTIFG
        BIT_CLEAR(I2C_FLAG, I2C_START);
        break;

    case  8:                                            // Vector  8: STPIFG
        BIT_CLEAR(I2C_FLAG, I2C_STOP);
        break;

    case 10:                                            // Vector 10: RXIFG
        command = I2C_RX_BUFFER;
        //BIT_CLEAR(I2C_FLAG, I2C_RX);
        break;

    case 12:                                  			// Vector 12: TXIFG
        break;

    default: break;
    }
}
