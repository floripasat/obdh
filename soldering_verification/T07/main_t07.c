/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
#include "driverlib.h"

//*****************************************************************************
//! This example shows how to configure the I2C module as a master for
//! single byte transmission in interrupt mode. The address of the slave
//! module that the master is communicating with also set in this example.
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - I2C peripheral
//! - GPIO Port peripheral (for I2C pins)
//! - SCL2
//! - SDA
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - USCI_B0_VECTOR.
//!
//
//*****************************************************************************
//*****************************************************************************
//
//Set the address for slave module. This is a 7-bit address sent in the
//following format:
//[A6:A5:A4:A3:A2:A1:A0:RS]
//
//A zero in the "RS" position of the first byte means that the master
//transmits (sends) data to the selected slave, and a one in this position
//means that the master receives data from the slave.
//
//*****************************************************************************
#define SLAVE_ADDRESS 0x48

uint8_t transmitData;

void main(void)
{
    //Stop WDT
    WDT_A_hold(WDT_A_BASE);

    //Assign I2C pins to USCI_B0
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P9,
        GPIO_PIN5 + GPIO_PIN6
        );

    //Initialize transmit data packet
    transmitData = 0x56;

    //Initialize Master
    USCI_B_I2C_initMasterParam param = {0};
    param.selectClockSource = USCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = UCS_getSMCLK();
    param.dataRate = USCI_B_I2C_SET_DATA_RATE_400KBPS;
    USCI_B_I2C_initMaster(USCI_B2_BASE, &param);

    //Specify slave address
    USCI_B_I2C_setSlaveAddress(USCI_B2_BASE,
                               SLAVE_ADDRESS
                               );

    //Set in transmit mode
    USCI_B_I2C_setMode(USCI_B2_BASE,
                       USCI_B_I2C_TRANSMIT_MODE
                       );

    //Enable I2C Module to start operations
    USCI_B_I2C_enable(USCI_B2_BASE);

    //Enable TX interrupt
    USCI_B_I2C_clearInterrupt(USCI_B2_BASE,
                              USCI_B_I2C_TRANSMIT_INTERRUPT
                              );
    USCI_B_I2C_enableInterrupt(USCI_B2_BASE,
                               USCI_B_I2C_TRANSMIT_INTERRUPT
                               );

    while(1)
    {
        //Send single byte data.
        USCI_B_I2C_masterSendSingleByte(USCI_B2_BASE,
                                        transmitData
                                        );

        //Delay until transmission completes
        while(USCI_B_I2C_isBusBusy(USCI_B2_BASE))
        {
            ;
        }

        //Delay between each transaction
        __delay_cycles(50);

        //Increment transmit data counter
        transmitData++;
    }
}

//******************************************************************************
//
//This is the USCI_B0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B2_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B2_VECTOR)))
#endif
void USCI_B2_ISR(void)
{
    switch(__even_in_range(UCB2IV,12))
    {
    //Vector 12: Transmit buffer empty - TXIF
    case USCI_I2C_UCTXIFG:
    {
        __no_operation();
        break;
    }
    default:  break;
    }
}
