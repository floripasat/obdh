/*
 * HAL_obdh.h
 *
 *  Created on: 27 de abr de 2016
 *      Author: mario
 */

#ifndef DRIVER_HAL_OBDH_H_
#define DRIVER_HAL_OBDH_H_

#include <msp430.h>
#include "../driverlib/MSP430F5xx_6xx/driverlib.h"

//I2C SLAVE ADDRESS
#define MPU0_I2C_SLAVE_ADRESS      0x68
#define MPU1_I2C_SLAVE_ADRESS      0x69
#define EPS_I2C_SLAVE_ADRESS      0x13

//USCI CONNECTED DEVICES (DRIVERLIB)
#define IMU_BASE_ADDRESS USCI_B1_BASE

#define SYSLED_PIN          BIT4		// LED connected to P5.4 (p 32) - OBDH SYSTEM LED
#define SYSLED_PORT_DIR 	P5DIR
#define SYSLED_PORT_OUT    	P5OUT

#define PAYLOAD_ENABLE_PIN          BIT7		// Payload Enable Pin: P1.7 (p 41)
#define PAYLOAD_ENABLE_PORT_DIR		P1DIR
#define PAYLOAD_ENABLE_PORT_OUT		P1OUT

#define UART_DEBUG_RX		1
#define UART_DEBUG_TX		0


// TI Compiler pre-processor is overflowing in the multiplication of long delay times.
// eg. __delay_cycles(60 * DELAY_1_S_IN_CYCLES) is generating different results than __delay_cycles(DELAY_60_S_IN_CYCLES)
//
// Default Internal Clock: MCLK and SMCLK to 1.048576 MHz
// (Time elaps) = (# of cycles) / (frequency)
// 1 Cycle = 1.0 / 1.048576*10^6 seconds  @~1Mhz default clock


// @ 4Mhz internal ref, 1 cycle ~= 0.25 uS
// Delays below adjusted empiricaly based on tests/mesurements (to fix internal clock drift)
#define DELAY_100_uS_IN_CYCLES        385
#define DELAY_500_uS_IN_CYCLES       1985
#define DELAY_1_MS_IN_CYCLES	     3964
#define DELAY_5_MS_IN_CYCLES	    19883
#define DELAY_10_MS_IN_CYCLES	    39783
#define DELAY_50_MS_IN_CYCLES	   198900
#define DELAY_100_MS_IN_CYCLES	   397900
#define DELAY_150_MS_IN_CYCLES	   596850
#define DELAY_1_S_IN_CYCLES	      3979750
#define DELAY_5_S_IN_CYCLES	     20000000
#define DELAY_60_S_IN_CYCLES    960000000
#define DELAY_120_S_IN_CYCLES  1920000000


//TODO: watchdog max times are hardcoded in the main loop
//	  use global defines below, in the hal file.
//	  must reference the WD definitions, because WD delay
//	  have its own special unit and are not in cycles
//#define TIMESLOT_OBDH_MAX			WD_250_mSEC
//#define TIMESLOT_EPS_MAX			WD_250_mSEC
//#define TIMESLOT_IMU_MAX			WD_250_mSEC
//#define TIMESLOT_RADIO_MAX			WD_250_mSEC
//#define TIMESLOT_ENCODEFRAME_MAX	WD_250_mSEC
//#define TIMESLOT_UGHOST_MAX			WD_250_mSEC
//#define TIMESLOT_FLASH_MAX			WD_250_mSEC
//#define TIMESLOT_SLEEP_MAX			WD_250_mSEC



/*
 * FLASH MEMORY ADRESSES
 */

#define BANK0  0
#define BANK1  1
#define BANK2  2
#define BANK3  3

//128 KB banks
#define BANK0_ADDR      0x008000
#define BANK1_ADDR      0x028000
#define BANK2_ADDR      0x048000
#define BANK3_ADDR      0x068000
//128 B info segments
#define SEGA_ADDR       0x001800
#define SEGB_ADDR       0x001880
#define SEGC_ADDR       0x001900
#define SEGD_ADDR       0x001980
//512 B bootstrap segments
#define BSL0_ADDR       0x001600
#define BSL1_ADDR       0x001400
#define BSL2_ADDR       0x001200
#define BSL3_ADDR       0x001000
#define MASS_ERASE      0XFFFFFF

//first boot start adress
#define BOOT_ADDR               BANK1_ADDR
//flash pointer save adress
#define FLASH_PTR_ADDR          SEGC_ADDR
//overflow flag message adress
#define OVERFLOW_FLAG_ADDR      0x026000
//last adress that can write a data(beyond this will enter the overflow)
#define LAST_WRITE_ADDR         0x087FFF

/********************************************/



// Define some macros that allow us to direct-access the ADC12 calibration
// constants that we need. See device datasheet for the full TLV table memory
// mapping.




// Calibration constant for ADC 1.5-V Reference, Temp. Sensor 30°C
#define CALADC12_15V_30C        (*((unsigned int *)0x1A1A))

// Calibration constant for ADC 1.5-V Reference, Temp. Sensor 85°C
#define CALADC12_15V_85C        (*((unsigned int *)0x1A1C))


/*
 * IMU MEASUREMENTS RANGES
 */

#define IMU_ACC_RANGE	16.0
#define IMU_GYR_RANGE	2.0

/********************************************/


/*
 * PIN OBDH ENG MODEL V2
 */
/* PORT1 */
#define VCC_5V_EN_DIR P1DIR
#define VCC_5V_EN_OUT P1OUT
#define VCC_5V_EN_PIN BIT0

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

/* PORT2 (PORTMAP) */
#define SPI0_CLK_DIR P2DIR
#define SPI0_CLK_SEL P2SEL
#define SPI0_CLK_PIN BIT0

#define I2C0_SDA_DIR P2DIR
#define I2C0_SDA_SEL P2SEL
#define I2C0_SDA_PIN BIT1

#define I2C0_SCL_DIR P2DIR
#define I2C0_SCL_SEL P2SEL
#define I2C0_SCL_PIN BIT2

#define SPI0_CSn_DIR P2DIR
#define SPI0_CSn_SEL P2SEL
#define SPI0_CSn_PIN BIT3

#define SPI0_MOSI_DIR P2DIR
#define SPI0_MOSI_SEL P2SEL
#define SPI0_MOSI_PIN BIT4

#define SPI0_MISO_DIR P2DIR
#define SPI0_MISO_SEL P2SEL
#define SPI0_MISO_PIN BIT5

#define uC_BEACON_2_DIR P2DIR
#define uC_BEACON_2_SEL P2SEL
#define uC_BEACON_2_PIN BIT6

#define uC_BEACON_3_DIR P2DIR
#define uC_BEACON_3_SEL P2SEL
#define uC_BEACON_3_PIN BIT7

/* PORT3 */
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

/* PORT4 */
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

#define uSDCard_CE_DIR P4DIR
#define uSDCard_CE_OUT P4OUT
#define uSDCard_CE_PIN BIT6


/* PORT5 */
#define VREF_DIR P5DIR
#define VREF_SEL P5SEL
#define VREF_PIN BIT0

#define AGND_DIR P5DIR
#define AGND_SEL P5SEL
#define AGND_PIN BIT1

#define PAYLOAD_1_EN_DIR P5DIR
#define PAYLOAD_1_EN_OUT P5OUT
#define PAYLOAD_1_EN_PIN BIT2

#define LED_SYSTEM_DIR P5DIR
#define LED_SYSTEM_OUT P5OUT
#define LED_SYSTEM_PIN BIT3

#define uC_BEACON_0_DIR P5DIR
#define uC_BEACON_0_SEL P5SEL
#define uC_BEACON_0_PIN BIT4

#define uC_BEACON_1_DIR P5DIR
#define uC_BEACON_1_SEL P5SEL
#define uC_BEACON_1_PIN BIT5

#define PAYLOAD_0_EN_DIR P5DIR
#define PAYLOAD_0_EN_OUT P5OUT
#define PAYLOAD_0_EN_PIN BIT6

/* PORT6 */
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


/* PORT7 */
#define XT2_N_DIR P7DIR
#define XT2_N_SEL P7SEL
#define XT2_N_PIN BIT2

#define XT2_P_DIR P7DIR
#define XT2_P_SEL P7SEL
#define XT2_P_PIN BIT3

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


/* PORT8 */
#define SPI1_CLK_DIR P8DIR
#define SPI1_CLK_SEL P8SEL
#define SPI1_CLK_PIN BIT1

#define SPI1_MOSI_DIR P8DIR
#define SPI1_MOSI_SEL P8SEL
#define SPI1_MOSI_PIN BIT2

#define SPI1_MISO_DIR P8DIR
#define SPI1_MISO_SEL P8SEL
#define SPI1_MISO_PIN BIT3

#define I2C1_SDA_DIR P8DIR
#define I2C1_SDA_SEL P8SEL
#define I2C1_SDA_PIN BIT5

#define I2C1_SCL_DIR P8DIR
#define I2C1_SCL_SEL P8SEL
#define I2C1_SCL_PIN BIT6

#define ANTENNA_DEBUG_DIR P8DIR
#define ANTENNA_DEBUG_OUT P8OUT
#define ANTENNA_DEBUG_PIN BIT7


/* PORT9 */
#define DRIVER_CH0_CH1_FAULT_DIR P9DIR
#define DRIVER_CH0_CH1_FAULT_IN P9IN
#define DRIVER_CH0_CH1_FAULT_PIN BIT0

#define DRIVER_CH2_FAULT_DIR P9DIR
#define DRIVER_CH2_FAULT_IN P9IN
#define DRIVER_CH2_FAULT_PIN BIT1

#define UART0_TX_DIR P9DIR
#define UART0_TX_SEL P9SEL
#define UART0_TX_PIN BIT2

#define UART0_RX_DIR P9DIR
#define UART0_RX_SEL P9SEL
#define UART0_RX_PIN BIT3

#define WDI_EXT_DIR P9DIR
#define WDI_EXT_OUT P9OUT
#define WDI_EXT_PIN BIT4

#define I2C2_SDA_DIR P9DIR
#define I2C2_SDA_SEL P9SEL
#define I2C2_SDA_PIN BIT5

#define I2C2_SCL_DIR P9DIR
#define I2C2_SCL_SEL P9SEL
#define I2C2_SCL_PIN BIT6

#define MR_WDog_DIR P9DIR
#define MR_WDog_OUT P9OUT
#define MR_WDog_PIN BIT7


/* ADC INPUT CHANNEL */
#define X_SUNSEN_ADC_CH 0
#define Y_SUNSEN_ADC_CH 1
#define Z_SUNSEN_ADC_CH 2
#define OBDH_CURRENT_ADC_CH 3
#define VREF_TEMP_ADC_CH 4
#define VCC_3V3_ADC_CH 5
#define INTERNAL_TEMP_SENSOR_ADC_CH 6


#endif /* DRIVER_HAL_OBDH_H_ */


