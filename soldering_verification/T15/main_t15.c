/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
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
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430F665x Demo - ADC12, Using an External Reference
//
//  Description: This example shows how to use an external positive reference for
//  the ADC12.The external reference is applied to the VeREF+ pin. AVss is used
//  for the negative reference. A single conversion is performed on channel A0.
//  The conversion results are stored in ADC12MEM0 and Test by applying a voltage
//  to channel A0, then setting and running to a break point at the "_NOP()"
//  instruction. To view the conversion results,open register window in debugger
//  and view the contents of ADC12MEM0.
//  NOTE: VeREF+ range: 1.4V (min) to AVCC (max)
//        VeREF- range: 0V (min) to 1.2V (max)
//
//
//                MSP430F665x
//             -------------------
//         /|\|                   |
//          | |                   |
//          --|RST                |
//            |                   |
//     Vin -->|P6.0/CB0/A0        |
//            |                   |
//     REF -->|P5.0/VREF+/VeREF+  |
//            |                   |
//
//
//   P. Thanigai
//   Texas Instruments Inc.
//   May 2012
//   Built with IAR Embedded Workbench Version: 5.40 & CCS V5.2
//******************************************************************************
#include <msp430.h>

// Calibration constant for ADC 1.5-V Reference, Temp. Sensor 30�C
#define CALADC12_15V_30C        (*((unsigned int *)0x1A1A))

// Calibration constant for ADC 1.5-V Reference, Temp. Sensor 85�C
#define CALADC12_15V_85C        (*((unsigned int *)0x1A1C))


int main(void)
{
  volatile unsigned int curr_raw;
  volatile float curr, curr_T = 0;
  int cont = 0;

  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer

  P6SEL |= BIT3;
  ADC12CTL0 = ADC12SHT0_15 | ADC12REFON | ADC12ON;
                                              // Internal ref = 1.5V
  ADC12CTL1 = ADC12SHP;                     // enable sample timer

  ADC12MCTL0 = ADC12SREF_2 | ADC12INCH_3;                 // Vr+ = VeREF+ (ext) and Vr-=AVss
  ADC12CTL0 |= ADC12ENC;                    // Enable conversions
  P5DIR |= BIT3;

  while (1)
  {
      while(ADC12CTL1 & ADC12BUSY);
      ADC12CTL0 &= ~ADC12SC;
      ADC12CTL0 |= ADC12SC;
      while (!(ADC12IFG & BIT0));
      curr_raw = ADC12MEM0;

      curr = ((float)curr_raw) * (3.0 / (4095.0 * 47000 * 0.005 * 0.05));

      curr_T = curr_T + curr_raw;
      if(cont++ % 64 == 0){
          curr_T = curr_T / 64.0;
        P5OUT ^= BIT3;
        curr_T = 0;
      }

    __delay_cycles(1000);
    __no_operation();                       // SET BREAKPOINT HERE
  }
}
