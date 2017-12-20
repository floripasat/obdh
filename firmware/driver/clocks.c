/*
 * clock.c
 *
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
 *
 * This file is part of FloripaSat-OBDH.
 *
 * FloripaSat-OBDH is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FloripaSat-OBDH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /**
 * \file clock.c
 *
 * \brief Configuration of the MSP430's clocks
 *
 * \author Matheus Frata
 *
 */

#include <driver/clocks.h>

volatile uint16_t status;



void setup_dco(){

    __bis_SR_register(SCG0);                  /**< Disable the FLL control loop     */
    UCSCTL0 = 0x0000;                         /**< Set lowest possible DCOx, MODx   */
    UCSCTL1 = DCORSEL_6;                      /**< Select DCO range 32MHz operation */
    UCSCTL2 = FLLD_0 + 975;                   /**< Set DCO Multiplier for 32MHz     */
                                              /**< (N + 1) * FLLRef = Fdco          */
                                              /**< (975 + 1) * 32768 = 32MHz        */
                                              /**< Set FLL Div = fDCOCLK/(1+0)      */
    UCSCTL3 = SELREF__REFOCLK | FLLREFDIV_0;
    __bic_SR_register(SCG0);                  /**< Enable the FLL control loop      */
}

void setup_xt1_xt2(void){

    while(BAKCTL & LOCKBAK)                   /**< Unlock XT1 pins for operation    */
        BAKCTL &= ~(LOCKBAK);

    XT2_N_DIR &= ~(XT2_N_PIN | XT2_P_PIN);
    XT2_N_SEL |= XT2_N_PIN | XT2_P_PIN;
    UCSCTL6 &= ~(XT1OFF | XT2OFF);            /**< Enable XT1 & XT2 */
    UCSCTL6 |= XCAP_3;                        /**< Internal load cap */
}

void setup_clk_sources(void){
//    P1DIR |= BIT0;    /**< ACLK set out to pin */
//    P1SEL |= BIT0;

//    P3DIR |= BIT4;    /**< SMCLK set out to pin */
//    P3SEL |= BIT4;

    UCSCTL5 |= DIVA__1 + DIVM__2 + DIVS__2;
    UCSCTL4 |= SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK;        /**< SMCLK = MCLK = XT2 , ACLK = XT1 */
}

uint8_t test_fault_flags(void){
    uint16_t attempts = 0;
    uint8_t result = 0xFF;

    do {
        _delay_cycles(16000);                                       /**< 1ms delay (DCO at ~16MHz) */
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | XT1HFOFFG | DCOFFG);     /**< Clear XT2,XT1,DCO fault flags */
        SFRIFG1 &= ~OFIFG;                                          /**< Clear fault flags */
        attempts++;
    } while ((SFRIFG1 & OFIFG) && attempts < XT_MAX_STARTUP_TIME ); /**< try for 10 seconds. The datasheet says 1s of typical Startup time. */

    if( attempts < XT_MAX_STARTUP_TIME){
        result = TEST_SUCESS;
    }
    else {
        result = TEST_FAIL;
    }
    return result;
}


uint16_t set_v_core_up (uint8_t level)
  {
    uint16_t PMMRIE_backup,SVSMHCTL_backup;

    /**< Open PMM registers for write access */
    PMMCTL0_H = 0xA5;

    /**< Disable dedicated Interrupts to prevent that needed flags will be cleared */
    PMMRIE_backup = PMMRIE;
    PMMRIE &= ~(SVSMHDLYIE | SVSMLDLYIE | SVMLVLRIE | SVMHVLRIE | SVMHVLRPE);
    SVSMHCTL_backup = SVSMHCTL;
    PMMIFG &= ~(SVMHIFG | SVSMHDLYIFG);
    /**< Set SVM highside to new level and check if a VCore increase is possible */
    SVSMHCTL = SVMHE | SVSHE | (SVSMHRRL0 * level);
    /**< Wait until SVM highside is settled */
    while ((PMMIFG & SVSMHDLYIFG) == 0);
    /**< Check if a VCore increase is possible */
    if ((PMMIFG & SVMHIFG) == SVMHIFG){       /**< Vcc is to low for a Vcore increase */
      /**< recover the previous settings */
      PMMIFG &= ~SVSMHDLYIFG;
      SVSMHCTL = SVSMHCTL_backup;
      /**< Wait until SVM highside is settled */
      while ((PMMIFG & SVSMHDLYIFG) == 0);
      /**< Clear all Flags */
      PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG | SVMLVLRIFG | SVMLIFG | SVSMLDLYIFG);
      /**< backup PMM-Interrupt-Register */
      PMMRIE = PMMRIE_backup;

      /**< Lock PMM registers for write access */
      PMMCTL0_H = 0x00;
      return PMM_STATUS_ERROR;            /**< return: voltage not set */
    }
    /**< Set also SVS highside to new level      -> Vcc is high enough for a Vcore increase */
    SVSMHCTL |= (SVSHRVL0 * level);
    /**< Set SVM low side to new level */
    SVSMLCTL = SVMLE | (SVSMLRRL0 * level);
    /**< Wait until SVM low side is settled */
    while ((PMMIFG & SVSMLDLYIFG) == 0);
    /**< Clear already set flags */
    PMMIFG &= ~(SVMLVLRIFG | SVMLIFG);
    /**< Set VCore to new level */
    PMMCTL0_L = PMMCOREV0 * level;
    /**< Wait until new level reached */
    if (PMMIFG & SVMLIFG)
      while ((PMMIFG & SVMLVLRIFG) == 0);
    /**< Set also SVS/SVM low side to new level */
    PMMIFG &= ~SVSMLDLYIFG;
    SVSMLCTL |= SVSLE | (SVSLRVL0 * level);
    /**< wait for lowside delay flags */
    while ((PMMIFG & SVSMLDLYIFG) == 0);

    /**< Disable SVS/SVM Low */
    /**< Disable full-performance mode to save energy */
    SVSMLCTL &= ~(_HAL_PMM_SVSLE + _HAL_PMM_SVMLE + _HAL_PMM_SVSFP + _HAL_PMM_SVMFP);
    SVSMHCTL &= ~(_HAL_PMM_SVSFP + _HAL_PMM_SVMFP);

    /**< Clear all Flags */
    PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG | SVMLVLRIFG | SVMLIFG | SVSMLDLYIFG);
    /**< backup PMM-Interrupt-Register */
    PMMRIE = PMMRIE_backup;

    /**< Lock PMM registers for write access */
    PMMCTL0_H = 0x00;
    return PMM_STATUS_OK;                               /**< return: OK */
  }

uint16_t set_v_core (uint8_t level)
{
  uint16_t actlevel;
  uint16_t status = 0;
  level &= PMMCOREV_3;                       /**< Set Mask for Max. level */
  actlevel = (PMMCTL0 & PMMCOREV_3);         /**< Get actual VCore */

  while (((level != actlevel) && (status == 0)) || (level < actlevel))      /**< step by step increase or decrease */
  {
    if (level > actlevel)
      status = set_v_core_up(++actlevel);
  }
  return status;
}

uint8_t clocks_setup(void){
    uint8_t test_flag;
    //TODO: Verify if it's necessary to set the Vcore
    set_v_core(OBDH_V_CORE);
    setup_dco();
    setup_xt1_xt2();
    setup_clk_sources();
    test_flag = test_fault_flags();

    return test_flag;
}
