/*
 * HAL_obdh.h
 *
 *  Created on: 27 de abr de 2016
 *      Author: mario
 */

#ifndef DRIVER_HAL_OBDH_H_
#define DRIVER_HAL_OBDH_H_

#include <msp430.h>
#include "../hw_memmap.h"

//I2C SLAVE ADDRESS
#define MPU0_I2C_SLAVE_ADRESS      0x68
#define MPU1_I2C_SLAVE_ADRESS      0x69
#define EPS_I2C_SLAVE_ADRESS      0x13

//USCI CONNECTED DEVICES (DRIVERLIB)
#define IMU_BASE_ADDRESS USCI_B1_BASE
#define EPS_BASE_ADDRESS USCI_B0_BASE


// @ 16Mhz internal ref, 1 cycle ~= 62.5 nS
// Delays below adjusted empiricaly based on tests/mesurements (to fix internal clock drift)
#define DELAY_100_uS_IN_CYCLES        1540
#define DELAY_500_uS_IN_CYCLES        7940
#define DELAY_1_MS_IN_CYCLES	     15856
#define DELAY_5_MS_IN_CYCLES	     79532
#define DELAY_10_MS_IN_CYCLES	    159132
#define DELAY_50_MS_IN_CYCLES	    795600
#define DELAY_100_MS_IN_CYCLES	   1591600
#define DELAY_150_MS_IN_CYCLES	   2387400
#define DELAY_1_S_IN_CYCLES	      16000000
#define DELAY_5_S_IN_CYCLES	      80000000
#define DELAY_60_S_IN_CYCLES     960000000
#define DELAY_120_S_IN_CYCLES   1920000000


/********************************************/

// Define some macros that allow us to direct-access the ADC12 calibration
// constants that we need. See device datasheet for the full TLV table memory
// mapping.

// Calibration constant for ADC 1.5-V Reference, Temp. Sensor 30°C
#define CALADC12_15V_30C        (*((unsigned int *)0x1A1A))
// Calibration constant for ADC 1.5-V Reference, Temp. Sensor 85°C
#define CALADC12_15V_85C        (*((unsigned int *)0x1A1C))


/********************************************/

#define     TEST_SUCESS     0x01
#define     TEST_FAIL       0x00

#define BIT_TOGGLE(REG, BIT)    (REG ^= BIT)
#define BIT_SET(REG, BIT)       (REG |= BIT)
#define BIT_CLEAR(REG, BIT)     (REG &= ~BIT)

/*
 * PINOUT OBDH ENG MODEL V2
 */

/***************** SUPPLY ******************/
#define VCC_5V_EN_DIR P1DIR
#define VCC_5V_EN_OUT P1OUT
#define VCC_5V_EN_PIN BIT0

#define VREF_DIR P5DIR
#define VREF_SEL P5SEL
#define VREF_PIN BIT0

#define AGND_DIR P5DIR
#define AGND_SEL P5SEL
#define AGND_PIN BIT1

/***************** LED DEBUG ******************/
#define LED_SYSTEM_DIR P5DIR
#define LED_SYSTEM_OUT P5OUT
#define LED_SYSTEM_PIN BIT3


/***************** PAYLOADS ******************/
#define PAYLOAD_0_EN_DIR P5DIR
#define PAYLOAD_0_EN_OUT P5OUT
#define PAYLOAD_0_EN_PIN BIT6

#define PAYLOAD_1_EN_DIR P5DIR
#define PAYLOAD_1_EN_OUT P5OUT
#define PAYLOAD_1_EN_PIN BIT2

/***************** HF CRYSTAL ******************/
#define XT2_N_DIR P7DIR
#define XT2_N_SEL P7SEL
#define XT2_N_PIN BIT2

#define XT2_P_DIR P7DIR
#define XT2_P_SEL P7SEL
#define XT2_P_PIN BIT3


/***************** ANTENNA ******************/
#define ANTENNA_DEBUG_DIR P8DIR
#define ANTENNA_DEBUG_OUT P8OUT
#define ANTENNA_DEBUG_PIN BIT7


/***************** EXTERNAL WATCHDOG TIMER ******************/
#define WDI_EXT_DIR P9DIR
#define WDI_EXT_OUT P9OUT
#define WDI_EXT_PIN BIT4

#define MR_WDog_DIR P9DIR
#define MR_WDog_OUT P9OUT
#define MR_WDog_PIN BIT7


/***************** ADC12 ******************/
#define ADC_SEL     P6SEL

#define X_SUNSEN_ADC_CH             0
#define Y_SUNSEN_ADC_CH             1
#define Z_SUNSEN_ADC_CH             2
#define OBDH_CURRENT_ADC_CH         3
#define VREF_TEMP_ADC_CH            4
#define VCC_3V3_ADC_CH              5
#define INTERNAL_TEMP_SENSOR_ADC_CH 6

#define X_SUNSEN_ADC_DIR P6DIR
#define X_SUNSEN_ADC_SEL P6SEL
#define X_SUNSEN_ADC_PIN BIT0

#define Y_SUNSEN_ADC_DIR P6DIR
#define Y_SUNSEN_ADC_SEL P6SEL
#define Y_SUNSEN_ADC_PIN BIT1

#define Z_SUNSEN_ADC_DIR P6DIR
#define Z_SUNSEN_ADC_SEL P6SEL
#define Z_SUNSEN_ADC_PIN BIT2

#define OBDH_CURRENT_ADC_DIR P6DIR
#define OBDH_CURRENT_ADC_SEL P6SEL
#define OBDH_CURRENT_ADC_PIN BIT3

#define ADC_Vref_TEMP_DIR P6DIR
#define ADC_Vref_TEMP_SEL P6SEL
#define ADC_Vref_TEMP_PIN BIT4

#define VCC_3V3_DIR P6DIR
#define VCC_3V3_SEL P6SEL
#define VCC_3V3_PIN BIT5



/***************** I2C ******************/
#define I2C0_SEL P2SEL
#define I2C0_SDA BIT1
#define I2C0_SCL BIT2

#define I2C1_SEL P8SEL
#define I2C1_SDA BIT5
#define I2C1_SCL BIT6

#define I2C2_SEL P9SEL
#define I2C2_SDA BIT5
#define I2C2_SCL BIT6

#define I2C0_SDA_DIR P2DIR
#define I2C0_SDA_SEL P2SEL
#define I2C0_SDA_PIN BIT1

#define I2C0_SCL_DIR P2DIR
#define I2C0_SCL_SEL P2SEL
#define I2C0_SCL_PIN BIT2

#define I2C1_SDA_DIR P8DIR
#define I2C1_SDA_SEL P8SEL
#define I2C1_SDA_PIN BIT5

#define I2C1_SCL_DIR P8DIR
#define I2C1_SCL_SEL P8SEL
#define I2C1_SCL_PIN BIT6

#define I2C2_SDA_DIR P9DIR
#define I2C2_SDA_SEL P9SEL
#define I2C2_SDA_PIN BIT5

#define I2C2_SCL_DIR P9DIR
#define I2C2_SCL_SEL P9SEL
#define I2C2_SCL_PIN BIT6

/***************** SPI ******************/
#define SPI1_CLK_DIR P8DIR
#define SPI1_CLK_SEL P8SEL
#define SPI1_CLK_PIN BIT1

#define SPI1_MOSI_DIR P8DIR
#define SPI1_MOSI_SEL P8SEL
#define SPI1_MOSI_PIN BIT2

#define SPI1_MISO_DIR P8DIR
#define SPI1_MISO_SEL P8SEL
#define SPI1_MISO_PIN BIT3

#define SPI0_CLK_DIR P2DIR
#define SPI0_CLK_SEL P2SEL
#define SPI0_CLK_PIN BIT0

#define SPI0_MOSI_DIR P2DIR
#define SPI0_MOSI_SEL P2SEL
#define SPI0_MOSI_PIN BIT4

#define SPI0_MISO_DIR P2DIR
#define SPI0_MISO_SEL P2SEL
#define SPI0_MISO_PIN BIT5

#define SPI0_CSn_DIR P2DIR
#define SPI0_CSn_SEL P2SEL
#define SPI0_CSn_PIN BIT3


/***************** UART ******************/
#define UART0_SEL P9SEL
#define UART0_TX BIT2
#define UART0_RX BIT3

#define UART0_TX_DIR P9DIR
#define UART0_TX_SEL P9SEL
#define UART0_TX_PIN BIT2

#define UART0_RX_DIR P9DIR
#define UART0_RX_SEL P9SEL
#define UART0_RX_PIN BIT3


/***************** TTC ******************/
#define TTC_3V3_PA_EN_DIR P1DIR
#define TTC_3V3_PA_EN_OUT P1OUT
#define TTC_3V3_PA_EN_PIN BIT1

#define TTC_RESETn_MAIN_DIR P1DIR
#define TTC_RESETn_MAIN_OUT P1OUT
#define TTC_RESETn_MAIN_PIN BIT2

#define TTC_GPIO2_MAIN_DIR P1DIR
#define TTC_GPIO2_MAIN_OUT P1OUT
#define TTC_GPIO2_MAIN_PIN BIT3

#define TTC_GPIO1_MAIN_DIR P1DIR
#define TTC_GPIO1_MAIN_OUT P1OUT
#define TTC_GPIO1_MAIN_PIN BIT4

#define TTC_GPIO0_MAIN_DIR P1DIR
#define TTC_GPIO0_MAIN_OUT P1OUT
#define TTC_GPIO0_MAIN_PIN BIT5

#define TTC_CTRL_RF_SWT_TX_DIR P1DIR
#define TTC_CTRL_RF_SWT_TX_OUT P1OUT
#define TTC_CTRL_RF_SWT_TX_PIN BIT6

#define TTC_CTRL_RF_SWT_RX_DIR P1DIR
#define TTC_CTRL_RF_SWT_RX_OUT P1OUT
#define TTC_CTRL_RF_SWT_RX_PIN BIT7

#define uC_BEACON_0_DIR P5DIR
#define uC_BEACON_0_SEL P5SEL
#define uC_BEACON_0_PIN BIT4

#define uC_BEACON_1_DIR P5DIR
#define uC_BEACON_1_SEL P5SEL
#define uC_BEACON_1_PIN BIT5

#define uC_BEACON_2_DIR P2DIR
#define uC_BEACON_2_SEL P2SEL
#define uC_BEACON_2_PIN BIT6

#define uC_BEACON_3_DIR P2DIR
#define uC_BEACON_3_SEL P2SEL
#define uC_BEACON_3_PIN BIT7


/***************** MEMORIES ******************/
#define Mem_WP_DIR P7DIR
#define Mem_WP_OUT P7OUT
#define Mem_WP_PIN BIT4

#define Mem0_CE_DIR P7DIR
#define Mem0_CE_OUT P7OUT
#define Mem0_CE_PIN BIT5

#define Mem1_CE_DIR P7DIR
#define Mem1_CE_OUT P7OUT
#define Mem1_CE_PIN BIT6

#define Mem2_CE_DIR P7DIR
#define Mem2_CE_OUT P7OUT
#define Mem2_CE_PIN BIT7

#define uSDCard_CE_DIR P4DIR
#define uSDCard_CE_OUT P4OUT
#define uSDCard_CE_PIN BIT6

/***************** SENSORS_CS ******************/
#define GYRO_Z_CSn_DIR P4DIR
#define GYRO_Z_CSn_OUT P4OUT
#define GYRO_Z_CSn_PIN BIT0

#define TEMP_Z_CSn_DIR P4DIR
#define TEMP_Z_CSn_OUT P4OUT
#define TEMP_Z_CSn_PIN BIT1

#define GYRO_Y_CSn_DIR P4DIR
#define GYRO_Y_CSn_OUT P4OUT
#define GYRO_Y_CSn_PIN BIT2

#define TEMP_Y_CSn_DIR P4DIR
#define TEMP_Y_CSn_OUT P4OUT
#define TEMP_Y_CSn_PIN BIT3

#define GYRO_X_CSn_DIR P4DIR
#define GYRO_X_CSn_OUT P4OUT
#define GYRO_X_CSn_PIN BIT4

#define TEMP_X_CSn_DIR P4DIR
#define TEMP_X_CSn_OUT P4OUT
#define TEMP_X_CSn_PIN BIT5

/***************** CURRENT DRIVER ******************/
#define DRIVER_CH0_CH1_SLEEP_DIR P3DIR
#define DRIVER_CH0_CH1_SLEEP_OUT P3OUT
#define DRIVER_CH0_CH1_SLEEP_PIN BIT0

#define DRIVER_CH2_SLEEP_DIR P3DIR
#define DRIVER_CH2_SLEEP_OUT P3OUT
#define DRIVER_CH2_SLEEP_PIN BIT1

#define DRIVER_CH0_0_DIR P3DIR
#define DRIVER_CH0_0_OUT P3OUT
#define DRIVER_CH0_0_PIN BIT2

#define DRIVER_CH0_1_DIR P3DIR
#define DRIVER_CH0_1_OUT P3OUT
#define DRIVER_CH0_1_PIN BIT3

#define DRIVER_CH1_0_DIR P3DIR
#define DRIVER_CH1_0_OUT P3OUT
#define DRIVER_CH1_0_PIN BIT4

#define DRIVER_CH1_1_DIR P3DIR
#define DRIVER_CH1_1_OUT P3OUT
#define DRIVER_CH1_1_PIN BIT5

#define DRIVER_CH2_0_DIR P3DIR
#define DRIVER_CH2_0_OUT P3OUT
#define DRIVER_CH2_0_PIN BIT6

#define DRIVER_CH2_1_DIR P3DIR
#define DRIVER_CH2_1_OUT P3OUT
#define DRIVER_CH2_1_PIN BIT7

#define DRIVER_CH0_CH1_FAULT_DIR P9DIR
#define DRIVER_CH0_CH1_FAULT_IN P9IN
#define DRIVER_CH0_CH1_FAULT_PIN BIT0

#define DRIVER_CH2_FAULT_DIR P9DIR
#define DRIVER_CH2_FAULT_IN P9IN
#define DRIVER_CH2_FAULT_PIN BIT1


#endif /* DRIVER_HAL_OBDH_H_ */


