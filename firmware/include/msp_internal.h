/*
 * obdh.h
 *
 *  Created on: 30 de mai de 2016
 *      Author: mario
 */

// Interface with internal components of the OBDH SoC


#ifndef INCLUDE_MSP_INTERNAL_H_
#define INCLUDE_MSP_INTERNAL_H_

#include "../hal/obdh_hal.h"
#include "../driver/adc.h"
#include "../driver/flash.h"
#include "floripasat_def.h"
#include "stdint.h"

#define RESET_ADDR_FLASH    SEGA_ADDR

//Current sensing circuit definitions
#if HAL_VERSION == HAL_V2_0
#define RSENSE_VALUE 47000.0 //ohms
#define CURRENT_GAIN 0.005 // A/V
#define RL_VALUE 0.05 // A/V
#endif

#if HAL_VERSION == HAL_V2_1
#define RSENSE_VALUE 20000.0 //ohms
#define CURRENT_GAIN 0.025 // A/V
#define RL_VALUE 0.05 // A/V
#endif

//Voltage sensing circuit definitions
#if HAL_VERSION == HAL_V2_0
#define VOLTAGE_DIVISOR 1.0
#endif

#if HAL_VERSION == HAL_V2_1
#define VOLTAGE_DIVISOR 2.0
#endif

unsigned int obdhTemperatureBuffer;
volatile float temperatureDegC;
volatile float temperatureDegF;

/**
 * \fn obdh_temperature_convert(uint16_t temp_raw)
 * That function converts the 12-bits temperature into a single precision
 * float value, in degree Celsius.
 * \param temperature_raw is the adc12 output value of temperature sensor
 * \return float temperature value, in degree Celsius
 */
float obdh_temperature_convert(uint16_t temperature_raw);

/**
 * \fn obdh_temperature_read(void)
 * Read the internal temperature sensor of MSP, over ADC12.
 * \param None
 * \return temperature value, 12-bit formated, right-justified
 */
uint16_t obdh_temperature_read(void);

/**
 * \fn obdh_current_read(void)
 * Read the current sensing circuit, over ADC12.
 * \param None
 * \return current value, 12-bit formated, right-justified
 */
uint16_t obdh_current_read(void);

/**
 * \fn obdh_current_convert(uint16_t curr_raw)
 * That function converts the 12-bits current into a single precision
 * float value, in Amperes.
 * \param curr_raw is the adc12 output value of the current sensing circuit
 * \return float current value, in Amperes
 */
float obdh_current_convert(uint16_t curr_raw);

/**
 * \fn obdh_voltage_read(void)
 * Read the supply voltage sensing circuit, over ADC12.
 * \param None
 * \return voltage value, 12-bit formated, right-justified
 */
uint16_t obdh_voltage_read(void);

/**
 * \fn obdh_voltage_convert(uint16_t volt_raw)
 * That function converts the 12-bits voltage into a single precision
 * float value, in Volts.
 * \param volt_raw is the adc12 output value of the voltage sensing circuit
 * \return float voltage value, in Volts
 */
float obdh_voltage_convert(uint16_t volt_raw);

/**
 * \fn read_fault_flags(void)
 * Read the oscillators fault flags
 * \param None
 * \return A byte with 4 lsb refers to XT2, XT1HF, XT1LF and DCO fault flags
 */
uint8_t read_fault_flags(void);

/**
 * \fn read_reset_value(void)
 * Read the reset counter and cause from the MSP flash memory (segment A)
 * \param None
 * \return A 3-bytes reset counter (LSB,2SB,3SB) and a 1 byte reset cause(MSB)
 */
uint32_t read_reset_value(void);

/**
 * \fn update_reset_value(void)
 * Read the previous reset counter, increment it, read the last reset cause
 * and write the both in the flash memory
 * \param None
 * \return None
 */
void update_reset_value(void);

#endif /* INCLUDE_MSP_INTERNAL_H_ */
